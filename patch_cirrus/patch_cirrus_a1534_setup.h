// credits to davidjo for making speakers work on mbp14,3
// below is the setup for mb9,1

//this line is only needed for kernel 5.0 and below
#define AC_VERB_GET_STRIPE_CONTROL		0x0f24

static inline unsigned int snd_hda_codec_read_check(struct hda_codec *codec, hda_nid_t nid, int flags, unsigned int verb, unsigned int parm, unsigned int check_val, int srcidx)
{
        unsigned int retval;
        retval = snd_hda_codec_read(codec, nid, flags, verb, parm);

        if (retval == -1)
                return retval;

        if (retval != check_val)
                codec_dbg(codec, "command nid BAD read check return value at %d: 0x%08x 0x%08x\n",srcidx,retval,check_val);

        return retval;
}
static inline void cs_4208_vendor_coef_set(struct hda_codec *codec, unsigned int idx,
                                      unsigned int coef)
{
        struct cs_spec *spec = codec->spec;
        snd_hda_codec_read(codec, spec->vendor_nid, 0,
                            AC_VERB_GET_COEF_INDEX, 0);
        snd_hda_codec_write(codec, spec->vendor_nid, 0,
                            AC_VERB_SET_COEF_INDEX, idx);
        snd_hda_codec_write(codec, spec->vendor_nid, 0,
                            AC_VERB_SET_PROC_COEF, coef);
        snd_hda_codec_write(codec, spec->vendor_nid, 0,
                            AC_VERB_SET_COEF_INDEX, 0);
        // appears to return 0
}
static inline unsigned int cs_4208_vendor_coef_get(struct hda_codec *codec, unsigned int idx)
{
        struct cs_spec *spec = codec->spec;
        unsigned int retval;
        snd_hda_codec_read(codec, spec->vendor_nid, 0,
                            AC_VERB_GET_COEF_INDEX, 0);
        snd_hda_codec_write(codec, spec->vendor_nid, 0,
                            AC_VERB_SET_COEF_INDEX, idx);
        retval = snd_hda_codec_read(codec, spec->vendor_nid, 0,
                                  AC_VERB_GET_PROC_COEF, 0);
        snd_hda_codec_write(codec, spec->vendor_nid, 0,
                            AC_VERB_SET_COEF_INDEX, 0);
        return retval;
}
static inline unsigned int cs_4208_vendor_coef_set_mask(struct hda_codec *codec, unsigned int idx,
                                      unsigned int coef, unsigned int mask)
{
        struct cs_spec *spec = codec->spec;
        unsigned int retval;
        //unsigned int mask_coef;
        snd_hda_codec_read(codec, spec->vendor_nid, 0,
                            AC_VERB_GET_COEF_INDEX, 0);
        snd_hda_codec_write(codec, spec->vendor_nid, 0,
                            AC_VERB_SET_COEF_INDEX, idx);
        retval = snd_hda_codec_read(codec, spec->vendor_nid, 0,
                                  AC_VERB_GET_PROC_COEF, 0);
        snd_hda_codec_write(codec, spec->vendor_nid, 0,
                            AC_VERB_SET_COEF_INDEX, idx);
        //mask_coef = (retval & ~mask) | coef;
        snd_hda_codec_write(codec, spec->vendor_nid, 0,
                            AC_VERB_SET_PROC_COEF, coef);
        snd_hda_codec_write(codec, spec->vendor_nid, 0,
                            AC_VERB_SET_COEF_INDEX, 0);
        // appears to return 0
        // lets return the read value for checking
        return retval;
}
void snd_hda_coef_item(struct hda_codec *codec, u16 write_flag, hda_nid_t nid, u32 idx, u32 param, u32 retdata, int srcidx)
{
        if (write_flag == 2)
        {
                unsigned int retval = cs_4208_vendor_coef_set_mask(codec, idx, param, 0);
                if (retval != retdata)
                {
                        if (srcidx > 0)
                                codec_dbg(codec, "command nid BAD mask return value at %d: 0x%08x 0x%08x\n",srcidx,retval,retdata);
                        else
                                codec_dbg(codec, "command nid BAD mask return value: 0x%08x 0x%08x\n",retval,retdata);
                }
        }
        else if (write_flag == 1)
                cs_4208_vendor_coef_set(codec, idx, param);
        else
        {
                unsigned int retval = cs_4208_vendor_coef_get(codec, idx);
                if (retval != retdata)
                {
                        if (srcidx > 0)
                                codec_dbg(codec, "command nid BAD      return value at %d: 0x%08x 0x%08x\n",srcidx,retval,retdata);
                        else
                                codec_dbg(codec, "command nid BAD      return value: 0x%08x 0x%08x\n",retval,retdata);
                }
        }
}

/* reserved for future use
static int headphones_a1534 (struct hda_codec *codec) {
	int retval;
	codec_dbg(codec, "headphones_a1534 start");

        retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000000, 0x10134208, 1); // 0x000f0000

        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x80000000, 2); // 0x010f0900
        retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 3); // 0x011f0900
        retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 4); // 0x017f0900
        retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 5); // 0x018f0900
        retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 6); // 0x021f0900
        retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 7); // 0x022f0900

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500

        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 13); // 0x010f0500

        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01070500

        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 16); // 0x010f0500
        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01070503
        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 18); // 0x010f0500

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500


        snd_hda_codec_write(codec, 0x24, 0, 0x7f0, 0x00000000);

       // read  node 0x24 arg 1 0x0000 arg 2 0x001c arg 3 0x0000 arg 4 0x0000 data 0x00000000 (27)
 //       { 0, CS4208_VENDOR_NID, 0x001c, 0x0000, 0x00000000 }, //   coef read 27
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x001c, 0x0000, 0x00000000, 28 ); //   coef read 28

       // write node 0x24 arg 1 0x0000 arg 2 0x001c arg 3 0x0000 arg 4 0x0000 (31)
 //       { 1, CS4208_VENDOR_NID, 0x001c, 0x0000, 0x00000000 }, //   coef write 31
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x001c, 0x0000, 0x00000000, 32 ); //   coef write 32

       // read  node 0x24 arg 1 0x0000 arg 2 0x001f arg 3 0x0000 arg 4 0x0000 data 0x00000000 (35)
 //       { 0, CS4208_VENDOR_NID, 0x001f, 0x0000, 0x00000000 }, //   coef read 35
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x001f, 0x0000, 0x00000000, 36 ); //   coef read 36
//        vendor_verb
        snd_hda_codec_write(codec, 0x24, 0, 0x7f0, 0x000000ae);


       // read  node 0x24 arg 1 0x0000 arg 2 0x0000 arg 3 0x0000 arg 4 0x0000 data 0x000000c4 (41)
 //       { 0, CS4208_VENDOR_NID, 0x0000, 0x0000, 0x000000c4 }, //   coef read 41
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0000, 0x0000, 0x000000c4, 42 ); //   coef read 42

       // write node 0x24 arg 1 0x0000 arg 2 0x0000 arg 3 0x20c4 arg 4 0x0000 (45)
 //       { 1, CS4208_VENDOR_NID, 0x0000, 0x20c4, 0x00000000 }, //   coef write 45
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0000, 0x20c4, 0x00000000, 46 ); //   coef write 46

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
//       snd_hda:     gpio direction 1 0x31 in in out out in in in out


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000020); // 0x00171520
//       snd_hda:     gpio data 1 0x20


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
//       snd_hda:     gpio enable 1 0x37

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
//       snd_hda:     gpio direction 1 0x31 in in out out in in in out


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000030); // 0x00171530
//       snd_hda:     gpio data 1 0x30


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
//       snd_hda:     gpio enable 1 0x37

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500

        snd_hda_codec_write(codec, 0x07, 0, AC_VERB_SET_CONNECT_SEL, 0x00000001); // 0x00770101


        retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 84); // 0x018f0700

        snd_hda_codec_write(codec, 0x18, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01870500

        retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 87); // 0x018f0500

        snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301

       // read  node 0x24 arg 1 0x0000 arg 2 0x001c arg 3 0x0000 arg 4 0x0000 data 0x00000000 (88)
 //       { 0, CS4208_VENDOR_NID, 0x001c, 0x0000, 0x00000000 }, //   coef read 88
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x001c, 0x0000, 0x00000000, 89 ); //   coef read 89

       // write node 0x24 arg 1 0x0000 arg 2 0x001c arg 3 0x0000 arg 4 0x0000 (92)
 //       { 1, CS4208_VENDOR_NID, 0x001c, 0x0000, 0x00000000 }, //   coef write 92
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x001c, 0x0000, 0x00000000, 93 ); //   coef write 93

        retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 97); // 0x018f0700
        snd_hda_codec_write(codec, 0x18, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000004); // 0x01870704
//       snd_hda:     24 []



       // read  node 0x24 arg 1 0x0000 arg 2 0x0019 arg 3 0x0000 arg 4 0x0000 data 0x00004383 (99)
 //       { 0, CS4208_VENDOR_NID, 0x0019, 0x0000, 0x00004383 }, //   coef read 99
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0019, 0x0000, 0x00004383, 100 ); //   coef read 100

       // write node 0x24 arg 1 0x0000 arg 2 0x0019 arg 3 0xc383 arg 4 0x0000 (103)
 //       { 1, CS4208_VENDOR_NID, 0x0019, 0xc383, 0x00000000 }, //   coef write 103
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0019, 0xc383, 0x00000000, 104 ); //   coef write 104


        snd_hda_codec_write(codec, 0x24, 0, 0x7f0, 0x00000000);



       // write node 0x24 arg 1 0x0000 arg 2 0x0019 arg 3 0x8383 arg 4 0x0000 (111)
 //       { 1, CS4208_VENDOR_NID, 0x0019, 0x8383, 0x00000000 }, //   coef write 111
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0019, 0x8383, 0x00000000, 112 ); //   coef write 112


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
//       snd_hda:     gpio direction 1 0x31 in in out out in in in out


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000010); // 0x00171510
//       snd_hda:     gpio data 1 0x10


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
//       snd_hda:     gpio enable 1 0x37


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
//       snd_hda:     gpio direction 1 0x31 in in out out in in in out


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
//       snd_hda:     gpio data 1 0x00


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
//       snd_hda:     gpio enable 1 0x37


       // read  node 0x24 arg 1 0x0000 arg 2 0x0000 arg 3 0x0000 arg 4 0x0000 data 0x000020c4 (130)
 //       { 0, CS4208_VENDOR_NID, 0x0000, 0x0000, 0x000020c4 }, 
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0000, 0x0000, 0x000020c4, 131 ); 
       // write node 0x24 arg 1 0x0000 arg 2 0x0000 arg 3 0x00c4 arg 4 0x0000 (134)
 //       { 1, CS4208_VENDOR_NID, 0x0000, 0x00c4, 0x00000000 }, 
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0000, 0x00c4, 0x00000000, 135 ); 
       snd_hda_codec_write(codec, 0x07, 0, AC_VERB_SET_CONNECT_SEL, 0x00000001); // 0x00770101


        retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000004, 141); // 0x018f0700
        snd_hda_codec_write(codec, 0x18, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000020); // 0x01870720
//       snd_hda:     24 ['AC_PINCTL_IN_EN']


        snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301

       // read  node 0x24 arg 1 0x0000 arg 2 0x001c arg 3 0x0000 arg 4 0x0000 data 0x00000000 (143)
 //       { 0, CS4208_VENDOR_NID, 0x001c, 0x0000, 0x00000000 }, //   coef read 143
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x001c, 0x0000, 0x00000000, 144 ); //   coef read 144

       // write node 0x24 arg 1 0x0000 arg 2 0x001c arg 3 0x0010 arg 4 0x0000 (147)
 //       { 1, CS4208_VENDOR_NID, 0x001c, 0x0010, 0x00000000 }, //   coef write 147
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x001c, 0x0010, 0x00000000, 148 ); //   coef write 148

        retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000020, 152); // 0x018f0700

        snd_hda_codec_write(codec, 0x18, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01870503

        retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 154); // 0x018f0500
        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 157); // 0x010f0500

        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01070500

        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 160); // 0x010f0500
        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01070503
        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 162); // 0x010f0500



       // read  node 0x24 arg 1 0x0000 arg 2 0x0035 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (164)
 //       { 0, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000 }, //   coef read 164
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000, 165 ); //   coef read 165

       // write node 0x24 arg 1 0x0000 arg 2 0x0035 arg 3 0x0000 arg 4 0x0000 (168)
 //       { 1, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000 }, //   coef write 168
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000, 169 ); //   coef write 169

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004031); // 0x00224031
//       snd_hda:     stream format 2 [('CHAN', 2), ('RATE', 44100), ('BITS', 24), ('RATE_MUL', 1), ('RATE_DIV', 1)]

        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 174); // 0x002f0500

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00270500

        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000030, 177); // 0x002f0500

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000010); // 0x00270610
//       snd_hda:     conv stream channel map 2 [('CHAN', 0), ('STREAMID', 1)]

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00270503

        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 180); // 0x002f0500

        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_CONNECT_SEL, 0x00000000); // 0x01070100

        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x0000a000, 0x000000c2, 183); // 0x010ba000
//       snd_hda:     amp gain/mute 16 0xa000 index 0x00 left/right 1 left output/input 1 output
//       snd_hda:     amp gain/mute 16 0x00c2 mute 1 gain 0x42 66
        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000a0c1); // 0x0103a0c1
//       snd_hda:     amp gain/mute 16 0xa0c1 mute 1 gain 0x41 65 index 0x00 left 1 right 0 output 1 input 0 left  output 
        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00008000, 0x000000c2, 185); // 0x010b8000
//       snd_hda:     amp gain/mute 16 0x8000 index 0x00 left/right 0 right output/input 1 output
//       snd_hda:     amp gain/mute 16 0x00c2 mute 1 gain 0x42 66
        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x000090c1); // 0x010390c1
//       snd_hda:     amp gain/mute 16 0x90c1 mute 1 gain 0x41 65 index 0x00 left 0 right 1 output 1 input 0  right output 

        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x0000a000, 0x000000c1, 187); // 0x010ba000
//       snd_hda:     amp gain/mute 16 0xa000 index 0x00 left/right 1 left output/input 1 output
//       snd_hda:     amp gain/mute 16 0x00c1 mute 1 gain 0x41 65
        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000a041); // 0x0103a041
//       snd_hda:     amp gain/mute 16 0xa041 mute 0 gain 0x41 65 index 0x00 left 1 right 0 output 1 input 0 left  output 
        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00008000, 0x000000c1, 189); // 0x010b8000
//       snd_hda:     amp gain/mute 16 0x8000 index 0x00 left/right 0 right output/input 1 output
//       snd_hda:     amp gain/mute 16 0x00c1 mute 1 gain 0x41 65
        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00009041); // 0x01039041
//       snd_hda:     amp gain/mute 16 0x9041 mute 0 gain 0x41 65 index 0x00 left 0 right 1 output 1 input 0  right output 

        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x0000a000, 0x00000041, 191); // 0x010ba000
//       snd_hda:     amp gain/mute 16 0xa000 index 0x00 left/right 1 left output/input 1 output
//       snd_hda:     amp gain/mute 16 0x0041 mute 0 gain 0x41 65
        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000a041); // 0x0103a041
//       snd_hda:     amp gain/mute 16 0xa041 mute 0 gain 0x41 65 index 0x00 left 1 right 0 output 1 input 0 left  output 
        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00008000, 0x00000041, 193); // 0x010b8000
//       snd_hda:     amp gain/mute 16 0x8000 index 0x00 left/right 0 right output/input 1 output
//       snd_hda:     amp gain/mute 16 0x0041 mute 0 gain 0x41 65
        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00009041); // 0x01039041
//       snd_hda:     amp gain/mute 16 0x9041 mute 0 gain 0x41 65 index 0x00 left 0 right 1 output 1 input 0  right output 

        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x0000a000, 0x000000ff, 195); // 0x002ba000
//       snd_hda:     amp gain/mute 2 0xa000 index 0x00 left/right 1 left output/input 1 output
//       snd_hda:     amp gain/mute 2 0x00ff mute 1 gain 0x7f 127
        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000a080); // 0x0023a080
//       snd_hda:     amp gain/mute 2 0xa080 mute 1 gain 0x0 0 index 0x00 left 1 right 0 output 1 input 0 left  output 
        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00008000, 0x000000ff, 197); // 0x002b8000
//       snd_hda:     amp gain/mute 2 0x8000 index 0x00 left/right 0 right output/input 1 output
//       snd_hda:     amp gain/mute 2 0x00ff mute 1 gain 0x7f 127
        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00009080); // 0x00239080
//       snd_hda:     amp gain/mute 2 0x9080 mute 1 gain 0x0 0 index 0x00 left 0 right 1 output 1 input 0  right output 

        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x0000a000, 0x00000080, 199); // 0x002ba000
//       snd_hda:     amp gain/mute 2 0xa000 index 0x00 left/right 1 left output/input 1 output
//       snd_hda:     amp gain/mute 2 0x0080 mute 1 gain 0x0 0
        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000a080); // 0x0023a080
//       snd_hda:     amp gain/mute 2 0xa080 mute 1 gain 0x0 0 index 0x00 left 1 right 0 output 1 input 0 left  output 
        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00008000, 0x00000080, 201); // 0x002b8000
//       snd_hda:     amp gain/mute 2 0x8000 index 0x00 left/right 0 right output/input 1 output
//       snd_hda:     amp gain/mute 2 0x0080 mute 1 gain 0x0 0
        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00009080); // 0x00239080
//       snd_hda:     amp gain/mute 2 0x9080 mute 1 gain 0x0 0 index 0x00 left 0 right 1 output 1 input 0  right output 

        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 203); // 0x010f0700
        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000040); // 0x01070740
//       snd_hda:     16 ['AC_PINCTL_OUT_EN']



       // read  node 0x24 arg 1 0x0000 arg 2 0x0035 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (206)
 //       { 0, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000 }, //   coef read 206
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000, 207 ); //   coef read 207

       // write node 0x24 arg 1 0x0000 arg 2 0x0035 arg 3 0x0000 arg 4 0x0000 (210)
 //       { 1, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000 }, //   coef write 210
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000, 211 ); //   coef write 211

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004031); // 0x00224031
//       snd_hda:     stream format 2 [('CHAN', 2), ('RATE', 44100), ('BITS', 24), ('RATE_MUL', 1), ('RATE_DIV', 1)]


        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00008000, 0x00000080, 218); // 0x002b8000
//       snd_hda:     amp gain/mute 2 0x8000 index 0x00 left/right 0 right output/input 1 output
//       snd_hda:     amp gain/mute 2 0x0080 mute 1 gain 0x0 0

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x000090ff); // 0x002390ff
//       snd_hda:     amp gain/mute 2 0x90ff mute 1 gain 0x7f 127 index 0x00 left 0 right 1 output 1 input 0  right output 


        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x0000a000, 0x00000080, 221); // 0x002ba000
//       snd_hda:     amp gain/mute 2 0xa000 index 0x00 left/right 1 left output/input 1 output
//       snd_hda:     amp gain/mute 2 0x0080 mute 1 gain 0x0 0

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000a0ff); // 0x0023a0ff
//       snd_hda:     amp gain/mute 2 0xa0ff mute 1 gain 0x7f 127 index 0x00 left 1 right 0 output 1 input 0 left  output 


        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00008000, 0x000000ff, 224); // 0x002b8000
//       snd_hda:     amp gain/mute 2 0x8000 index 0x00 left/right 0 right output/input 1 output
//       snd_hda:     amp gain/mute 2 0x00ff mute 1 gain 0x7f 127

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000907f); // 0x0023907f
//       snd_hda:     amp gain/mute 2 0x907f mute 0 gain 0x7f 127 index 0x00 left 0 right 1 output 1 input 0  right output 


        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x0000a000, 0x000000ff, 227); // 0x002ba000
//       snd_hda:     amp gain/mute 2 0xa000 index 0x00 left/right 1 left output/input 1 output
//       snd_hda:     amp gain/mute 2 0x00ff mute 1 gain 0x7f 127

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000a07f); // 0x0023a07f
//       snd_hda:     amp gain/mute 2 0xa07f mute 0 gain 0x7f 127 index 0x00 left 1 right 0 output 1 input 0 left  output 


        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x0000a000, 0x0000007f, 230); // 0x002ba000
//       snd_hda:     amp gain/mute 2 0xa000 index 0x00 left/right 1 left output/input 1 output
//       snd_hda:     amp gain/mute 2 0x007f mute 0 gain 0x7f 127
        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000a07f); // 0x0023a07f
//       snd_hda:     amp gain/mute 2 0xa07f mute 0 gain 0x7f 127 index 0x00 left 1 right 0 output 1 input 0 left  output 
        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00008000, 0x0000007f, 232); // 0x002b8000
//       snd_hda:     amp gain/mute 2 0x8000 index 0x00 left/right 0 right output/input 1 output
//       snd_hda:     amp gain/mute 2 0x007f mute 0 gain 0x7f 127
        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000907f); // 0x0023907f
//       snd_hda:     amp gain/mute 2 0x907f mute 0 gain 0x7f 127 index 0x00 left 0 right 1 output 1 input 0  right output 

        retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 235); // 0x006f0500
        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 236); // 0x010f0500
        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 237); // 0x002f0500
        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 240); // 0x010f0500

        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01070500

        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 243); // 0x010f0500
        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01070503
        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 245); // 0x010f0500

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500


        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00008000, 0x0000007f, 254); // 0x002b8000
//       snd_hda:     amp gain/mute 2 0x8000 index 0x00 left/right 0 right output/input 1 output
//       snd_hda:     amp gain/mute 2 0x007f mute 0 gain 0x7f 127

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000907f); // 0x0023907f
//       snd_hda:     amp gain/mute 2 0x907f mute 0 gain 0x7f 127 index 0x00 left 0 right 1 output 1 input 0  right output 

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500


        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x0000a000, 0x0000007f, 270); // 0x002ba000
//       snd_hda:     amp gain/mute 2 0xa000 index 0x00 left/right 1 left output/input 1 output
//       snd_hda:     amp gain/mute 2 0x007f mute 0 gain 0x7f 127

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000a07f); // 0x0023a07f
//       snd_hda:     amp gain/mute 2 0xa07f mute 0 gain 0x7f 127 index 0x00 left 1 right 0 output 1 input 0 left  output 

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503

        retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000000, 0x10134208, 274); // 0x000f0000

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500

        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01070500

        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 280); // 0x010f0500

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00270500
        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000030, 283); // 0x002f0500

        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000030, 286); // 0x010f0500
        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000030, 287); // 0x010f0500


       // read  node 0x24 arg 1 0x0000 arg 2 0x0035 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (288)
 //       { 0, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000 }, //   coef read 288
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000, 289 ); //   coef read 289

       // write node 0x24 arg 1 0x0000 arg 2 0x0035 arg 3 0x0000 arg 4 0x0000 (292)
 //       { 1, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000 }, //   coef write 292
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000, 293 ); //   coef write 293

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004031); // 0x00224031
//       snd_hda:     stream format 2 [('CHAN', 2), ('RATE', 44100), ('BITS', 24), ('RATE_MUL', 1), ('RATE_DIV', 1)]

        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000030, 298); // 0x002f0500

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000010); // 0x00270610
//       snd_hda:     conv stream channel map 2 [('CHAN', 0), ('STREAMID', 1)]

        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_CONNECT_SEL, 0x00000000); // 0x01070100

        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x0000a000, 0x00000041, 301); // 0x010ba000
//       snd_hda:     amp gain/mute 16 0xa000 index 0x00 left/right 1 left output/input 1 output
//       snd_hda:     amp gain/mute 16 0x0041 mute 0 gain 0x41 65
        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000a041); // 0x0103a041
//       snd_hda:     amp gain/mute 16 0xa041 mute 0 gain 0x41 65 index 0x00 left 1 right 0 output 1 input 0 left  output 
        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00008000, 0x00000041, 303); // 0x010b8000
//       snd_hda:     amp gain/mute 16 0x8000 index 0x00 left/right 0 right output/input 1 output
//       snd_hda:     amp gain/mute 16 0x0041 mute 0 gain 0x41 65
        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00009041); // 0x01039041
//       snd_hda:     amp gain/mute 16 0x9041 mute 0 gain 0x41 65 index 0x00 left 0 right 1 output 1 input 0  right output 

        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x0000a000, 0x00000041, 305); // 0x010ba000
//       snd_hda:     amp gain/mute 16 0xa000 index 0x00 left/right 1 left output/input 1 output
//       snd_hda:     amp gain/mute 16 0x0041 mute 0 gain 0x41 65
        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000a041); // 0x0103a041
//       snd_hda:     amp gain/mute 16 0xa041 mute 0 gain 0x41 65 index 0x00 left 1 right 0 output 1 input 0 left  output 
        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00008000, 0x00000041, 307); // 0x010b8000
//       snd_hda:     amp gain/mute 16 0x8000 index 0x00 left/right 0 right output/input 1 output
//       snd_hda:     amp gain/mute 16 0x0041 mute 0 gain 0x41 65
        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00009041); // 0x01039041
//       snd_hda:     amp gain/mute 16 0x9041 mute 0 gain 0x41 65 index 0x00 left 0 right 1 output 1 input 0  right output 

        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x0000a000, 0x00000041, 309); // 0x010ba000
//       snd_hda:     amp gain/mute 16 0xa000 index 0x00 left/right 1 left output/input 1 output
//       snd_hda:     amp gain/mute 16 0x0041 mute 0 gain 0x41 65
        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000a041); // 0x0103a041
//       snd_hda:     amp gain/mute 16 0xa041 mute 0 gain 0x41 65 index 0x00 left 1 right 0 output 1 input 0 left  output 
        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00008000, 0x00000041, 311); // 0x010b8000
//       snd_hda:     amp gain/mute 16 0x8000 index 0x00 left/right 0 right output/input 1 output
//       snd_hda:     amp gain/mute 16 0x0041 mute 0 gain 0x41 65
        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00009041); // 0x01039041
//       snd_hda:     amp gain/mute 16 0x9041 mute 0 gain 0x41 65 index 0x00 left 0 right 1 output 1 input 0  right output 

        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x0000a000, 0x0000007f, 313); // 0x002ba000
//       snd_hda:     amp gain/mute 2 0xa000 index 0x00 left/right 1 left output/input 1 output
//       snd_hda:     amp gain/mute 2 0x007f mute 0 gain 0x7f 127
        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000a000); // 0x0023a000
//       snd_hda:     amp gain/mute 2 0xa000 mute 0 gain 0x0 0 index 0x00 left 1 right 0 output 1 input 0 left  output 
        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00008000, 0x0000007f, 315); // 0x002b8000
//       snd_hda:     amp gain/mute 2 0x8000 index 0x00 left/right 0 right output/input 1 output
//       snd_hda:     amp gain/mute 2 0x007f mute 0 gain 0x7f 127
        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00009000); // 0x00239000
//       snd_hda:     amp gain/mute 2 0x9000 mute 0 gain 0x0 0 index 0x00 left 0 right 1 output 1 input 0  right output 

        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x0000a000, 0x00000000, 317); // 0x002ba000
//       snd_hda:     amp gain/mute 2 0xa000 index 0x00 left/right 1 left output/input 1 output
//       snd_hda:     amp gain/mute 2 0x0000 mute 0 gain 0x0 0
        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000a080); // 0x0023a080
//       snd_hda:     amp gain/mute 2 0xa080 mute 1 gain 0x0 0 index 0x00 left 1 right 0 output 1 input 0 left  output 
        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00008000, 0x00000000, 319); // 0x002b8000
//       snd_hda:     amp gain/mute 2 0x8000 index 0x00 left/right 0 right output/input 1 output
//       snd_hda:     amp gain/mute 2 0x0000 mute 0 gain 0x0 0
        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00009080); // 0x00239080
//       snd_hda:     amp gain/mute 2 0x9080 mute 1 gain 0x0 0 index 0x00 left 0 right 1 output 1 input 0  right output 

        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000040, 321); // 0x010f0700
        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000040); // 0x01070740
//       snd_hda:     16 ['AC_PINCTL_OUT_EN']


        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x0000a000, 0x00000080, 324); // 0x002ba000
//       snd_hda:     amp gain/mute 2 0xa000 index 0x00 left/right 1 left output/input 1 output
//       snd_hda:     amp gain/mute 2 0x0080 mute 1 gain 0x0 0
        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000a000); // 0x0023a000
//       snd_hda:     amp gain/mute 2 0xa000 mute 0 gain 0x0 0 index 0x00 left 1 right 0 output 1 input 0 left  output 
        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00008000, 0x00000080, 326); // 0x002b8000
//       snd_hda:     amp gain/mute 2 0x8000 index 0x00 left/right 0 right output/input 1 output
//       snd_hda:     amp gain/mute 2 0x0080 mute 1 gain 0x0 0
        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00009000); // 0x00239000
//       snd_hda:     amp gain/mute 2 0x9000 mute 0 gain 0x0 0 index 0x00 left 0 right 1 output 1 input 0  right output 


        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x0000a000, 0x00000000, 329); // 0x002ba000
//       snd_hda:     amp gain/mute 2 0xa000 index 0x00 left/right 1 left output/input 1 output
//       snd_hda:     amp gain/mute 2 0x0000 mute 0 gain 0x0 0

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000a000); // 0x0023a000
//       snd_hda:     amp gain/mute 2 0xa000 mute 0 gain 0x0 0 index 0x00 left 1 right 0 output 1 input 0 left  output 


        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00008000, 0x00000000, 332); // 0x002b8000
//       snd_hda:     amp gain/mute 2 0x8000 index 0x00 left/right 0 right output/input 1 output
//       snd_hda:     amp gain/mute 2 0x0000 mute 0 gain 0x0 0

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00009000); // 0x00239000
//       snd_hda:     amp gain/mute 2 0x9000 mute 0 gain 0x0 0 index 0x00 left 0 right 1 output 1 input 0  right output 


        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x0000a000, 0x00000000, 335); // 0x002ba000
//       snd_hda:     amp gain/mute 2 0xa000 index 0x00 left/right 1 left output/input 1 output
//       snd_hda:     amp gain/mute 2 0x0000 mute 0 gain 0x0 0

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000a07f); // 0x0023a07f
//       snd_hda:     amp gain/mute 2 0xa07f mute 0 gain 0x7f 127 index 0x00 left 1 right 0 output 1 input 0 left  output 


        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00008000, 0x00000000, 338); // 0x002b8000
//       snd_hda:     amp gain/mute 2 0x8000 index 0x00 left/right 0 right output/input 1 output
//       snd_hda:     amp gain/mute 2 0x0000 mute 0 gain 0x0 0

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000907f); // 0x0023907f
//       snd_hda:     amp gain/mute 2 0x907f mute 0 gain 0x7f 127 index 0x00 left 0 right 1 output 1 input 0  right output 

        retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 340); // 0x006f0500
        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 341); // 0x010f0500
        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 342); // 0x002f0500

	codec_dbg(codec, "headphones_a1534 end");
	return 0;
}
*/

static int setup_a1534 (struct hda_codec *codec) {
	int retval;
	codec_dbg(codec, "setup_a1534 start");

	retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000000, 0x10134208, 1); // 0x000f0000
	retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000002, 0x00100401, 2); // 0x000f0002
	retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000000, 0x10134208, 3); // 0x000f0000
	retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000002, 0x00100401, 4); // 0x000f0002
	retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000004, 0x00010001, 5); // 0x000f0004
	retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_PARAMETERS, 0x00000004, 0x00020023, 6); // 0x001f0004
	retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000101, 7); // 0x001f0005
	retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_GET_SUBSYSTEM_ID, 0x00000000, 0x106b6400, 8); // 0x001f2000
	retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000000, 0x10134208, 9); // 0x000f0000
	retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000002, 0x00100401, 10); // 0x000f0002
	retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000000, 0x80862808, 11); // 0x000f0000
	retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000002, 0x00100000, 12); // 0x000f0002
	retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_PARAMETERS, 0x0000000f, 0xe0000019, 13); // 0x001f000f
	//snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_DBL_CODEC_RESET, 0x00000000); // 0x001fff00
	retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000000, 0x10134208, 15); // 0x000f0000
	retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000002, 0x00100401, 16); // 0x000f0002
	retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000101, 17); // 0x001f0005
	retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_PARAMETERS, 0x0000000f, 0xe0000019, 18); // 0x001f000f
	retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x00000000, 19); // 0x001f000a
	retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000000, 20); // 0x001f000b
	retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 21); // 0x001f0012
	retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 22); // 0x001f000d
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_GET_SUBSYSTEM_ID, 0x00000000, 0x106b6400, 24); // 0x001f2000
	retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_PARAMETERS, 0x00000008, 0x00010000, 25); // 0x001f0008
	retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_GET_GPIO_DIRECTION, 0x00000000, 0x00000000, 26); // 0x001f1700
	retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_PARAMETERS, 0x00000011, 0xc0000206, 27); // 0x001f0011
	retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_PARAMETERS, 0x00000004, 0x00020023, 28); // 0x001f0004
	retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 29); // 0x002f0005
	retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_PARAMETERS, 0x00000009, 0x000d043d, 30); // 0x002f0009
	retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 31); // 0x002f000f
	retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e07f0, 32); // 0x002f000a
	retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 33); // 0x002f000b
	retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_PARAMETERS, 0x00000012, 0x80017f7f, 34); // 0x002f0012
	retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 35); // 0x002f000d
	retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_PARAMETERS, 0x00000009, 0x000d043d, 36); // 0x002f0009
	snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00270500
	retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e07f0, 38); // 0x002f000a
	retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 39); // 0x002f000b
	retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_PARAMETERS, 0x00000012, 0x80017f7f, 40); // 0x002f0012
	retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 41); // 0x002f000f
	retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_STRIPE_CONTROL, 0x00000000, 0x00100000, 42); // 0x002f2400
	snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00270503
	retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 45); // 0x003f0005
	retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_PARAMETERS, 0x00000009, 0x000d043d, 46); // 0x003f0009
	retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 47); // 0x003f000f
	retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e07f0, 48); // 0x003f000a
	retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 49); // 0x003f000b
	retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_PARAMETERS, 0x00000012, 0x80017f7f, 50); // 0x003f0012
	retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 51); // 0x003f000d
	retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_PARAMETERS, 0x00000009, 0x000d043d, 52); // 0x003f0009
	snd_hda_codec_write(codec, 0x03, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00370500
	retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e07f0, 54); // 0x003f000a
	retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 55); // 0x003f000b
	retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_PARAMETERS, 0x00000012, 0x80017f7f, 56); // 0x003f0012
	retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 57); // 0x003f000f
	retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_GET_STRIPE_CONTROL, 0x00000000, 0x00100000, 58); // 0x003f2400
	snd_hda_codec_write(codec, 0x03, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00370503
	retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 61); // 0x004f0005
	retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_PARAMETERS, 0x00000009, 0x000d043d, 62); // 0x004f0009
	retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 63); // 0x004f000f
	retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e07f0, 64); // 0x004f000a
	retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 65); // 0x004f000b
	retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_PARAMETERS, 0x00000012, 0x80017f7f, 66); // 0x004f0012
	retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 67); // 0x004f000d
	retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_PARAMETERS, 0x00000009, 0x000d043d, 68); // 0x004f0009
	snd_hda_codec_write(codec, 0x04, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00470500
	retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e07f0, 70); // 0x004f000a
	retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 71); // 0x004f000b
	retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_PARAMETERS, 0x00000012, 0x80017f7f, 72); // 0x004f0012
	retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 73); // 0x004f000f
	retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_GET_STRIPE_CONTROL, 0x00000000, 0x00100000, 74); // 0x004f2400
	snd_hda_codec_write(codec, 0x04, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00470503
	retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 77); // 0x005f0005
	retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_PARAMETERS, 0x00000009, 0x000d043d, 78); // 0x005f0009
	retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 79); // 0x005f000f
	retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e07f0, 80); // 0x005f000a
	retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 81); // 0x005f000b
	retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_PARAMETERS, 0x00000012, 0x80017f7f, 82); // 0x005f0012
	retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 83); // 0x005f000d
	retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_PARAMETERS, 0x00000009, 0x000d043d, 84); // 0x005f0009
	snd_hda_codec_write(codec, 0x05, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00570500
	retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e07f0, 86); // 0x005f000a
	retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 87); // 0x005f000b
	retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_PARAMETERS, 0x00000012, 0x80017f7f, 88); // 0x005f0012
	retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 89); // 0x005f000f
	retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_GET_STRIPE_CONTROL, 0x00000000, 0x00100000, 90); // 0x005f2400
	snd_hda_codec_write(codec, 0x05, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00570503
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 93); // 0x006f0005
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0018051b, 94); // 0x006f0009
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 95); // 0x006f000f
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e01f5, 96); // 0x006f000a
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 97); // 0x006f000b
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 98); // 0x006f0012
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x80033f33, 99); // 0x006f000d
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0018051b, 100); // 0x006f0009
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00670500
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e01f5, 102); // 0x006f000a
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 103); // 0x006f000b
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x80033f33, 104); // 0x006f000d
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000003, 105); // 0x006f000e
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x00191715, 106); // 0x006f0200
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 107); // 0x006f000f
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00670503
	retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 110); // 0x007f0005
	retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0018051b, 111); // 0x007f0009
	retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 112); // 0x007f000f
	retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e01f5, 113); // 0x007f000a
	retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 114); // 0x007f000b
	retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 115); // 0x007f0012
	retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x80033f33, 116); // 0x007f000d
	retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0018051b, 117); // 0x007f0009
	snd_hda_codec_write(codec, 0x07, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00770500
	retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e01f5, 119); // 0x007f000a
	retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 120); // 0x007f000b
	retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x80033f33, 121); // 0x007f000d
	retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000003, 122); // 0x007f000e
	retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x001a1816, 123); // 0x007f0200
	retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 124); // 0x007f000f
	snd_hda_codec_write(codec, 0x07, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00770503
	retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 127); // 0x008f0005
	retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0018051b, 128); // 0x008f0009
	retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 129); // 0x008f000f
	retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e01f5, 130); // 0x008f000a
	retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 131); // 0x008f000b
	retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 132); // 0x008f0012
	retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x80033f33, 133); // 0x008f000d
	retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0018051b, 134); // 0x008f0009
	snd_hda_codec_write(codec, 0x08, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00870500
	retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e01f5, 136); // 0x008f000a
	retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 137); // 0x008f000b
	retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x80033f33, 138); // 0x008f000d
	retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 139); // 0x008f000e
	retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x0000001b, 140); // 0x008f0200
	retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 141); // 0x008f000f
	snd_hda_codec_write(codec, 0x08, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00870503
	retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 144); // 0x009f0005
	retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0018051b, 145); // 0x009f0009
	retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 146); // 0x009f000f
	retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e01f5, 147); // 0x009f000a
	retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 148); // 0x009f000b
	retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 149); // 0x009f0012
	retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x80033f33, 150); // 0x009f000d
	retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0018051b, 151); // 0x009f0009
	snd_hda_codec_write(codec, 0x09, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00970500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_UNSOLICITED_ENABLE, 0x00000083); // 0x01070883
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000020); // 0x00171720
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000026); // 0x00171626
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000030); // 0x00171730
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000036); // 0x00171636
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 212); // 0x010f0900
	retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 213); // 0x011f0900
	retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 214); // 0x017f0900
	retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 215); // 0x018f0900
	retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 216); // 0x021f0900
	retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x80000000, 217); // 0x022f0900
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 265); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01070500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 268); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01070503
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 270); // 0x010f0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_UNSOLICITED_ENABLE, 0x00000083); // 0x01070883
	snd_hda_codec_write(codec, 0x18, 0, AC_VERB_SET_UNSOLICITED_ENABLE, 0x00000009); // 0x01870809
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004031); // 0x00624031
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 297); // 0x006f0500
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00670500
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000030, 300); // 0x006f0500
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00670600
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00670503
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 303); // 0x006f0500
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x000000b3, 314); // 0x006b2000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x000060a7); // 0x006360a7
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x000000b3, 316); // 0x006b0000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x000050a7); // 0x006350a7
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x000000a7, 318); // 0x006b2000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x000060a7); // 0x006360a7
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x000000a7, 320); // 0x006b0000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x000050a7); // 0x006350a7
	retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x00000000, 322); // 0x019b2000
	snd_hda_codec_write(codec, 0x19, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00006000); // 0x01936000
	retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x00000000, 324); // 0x019b0000
	snd_hda_codec_write(codec, 0x19, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00005000); // 0x01935000
	retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x00000000, 326); // 0x019b2000
	snd_hda_codec_write(codec, 0x19, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00006000); // 0x01936000
	retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x00000000, 328); // 0x019b0000
	snd_hda_codec_write(codec, 0x19, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00005000); // 0x01935000
	retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 330); // 0x019f0700
	snd_hda_codec_write(codec, 0x19, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000020); // 0x01970720
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004031); // 0x00624031
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x000000a7, 357); // 0x006b2000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x000060b3); // 0x006360b3
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x000000a7, 359); // 0x006b0000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x000050b3); // 0x006350b3
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x000000b3, 362); // 0x006b2000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00006033); // 0x00636033
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x000000b3, 364); // 0x006b0000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00005033); // 0x00635033
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 373); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004013); // 0x00a24013
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 375); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000001); // 0x00a70e01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 378); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 381); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000111, 382); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00a70600
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 385); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000111, 386); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000010); // 0x00a70d10
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 407); // 0x00af0600
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 408); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 409); // 0x00bf0600
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 410); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 411); // 0x00cf0600
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 412); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 413); // 0x00df0600
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 414); // 0x00df0500
	snd_hda_codec_write(codec, 0x0a, 0, 0x7f0, 0x00000003);
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 432); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 435); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000110, 436); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00a70600
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 440); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000111, 441); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000010); // 0x00a70d10
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 444); // 0x00bf0500
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00b70500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 447); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 448); // 0x00bf0d00
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00b70d01
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00b70600
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00b70503
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 452); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000001, 453); // 0x00bf0d00
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00b70d00
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 456); // 0x00cf0500
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00c70500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 459); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 460); // 0x00cf0d00
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00c70d01
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00c70600
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00c70503
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 464); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 465); // 0x00cf0d00
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 467); // 0x00df0500
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00d70500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 470); // 0x00df0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 471); // 0x00df0d00
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00d70d01
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00d70600
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00d70503
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 475); // 0x00df0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 476); // 0x00df0d00
	snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_CONNECT_SEL, 0x00000000); // 0x01d70100
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000001); // 0x00a70e01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 481); // 0x01df0700
	snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000040); // 0x01d70740
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000000); // 0x00a70e00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00a70d00
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 499); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 502); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 503); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00a70d01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00a70600
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 507); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000001, 508); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00a70d00
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 511); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_STREAM_FORMAT, 0x00000000); // 0x00a20000
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000040, 513); // 0x01df0700
	snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01d70700
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 516); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004013); // 0x00a24013
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 518); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000001); // 0x00a70e01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 539); // 0x00af0600
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 540); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 541); // 0x00bf0600
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 542); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 543); // 0x00cf0600
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 544); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 545); // 0x00df0600
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 546); // 0x00df0500
	snd_hda_codec_write(codec, 0x0a, 0, 0x7f0, 0x00000003);
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 564); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 567); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000111, 568); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00a70600
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 571); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000111, 572); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000010); // 0x00a70d10
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 575); // 0x00bf0500
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00b70500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 578); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 579); // 0x00bf0d00
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00b70d01
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00b70600
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00b70503
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 583); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000001, 584); // 0x00bf0d00
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00b70d00
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 587); // 0x00cf0500
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00c70500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 590); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 591); // 0x00cf0d00
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00c70d01
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00c70600
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00c70503
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 595); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 596); // 0x00cf0d00
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 598); // 0x00df0500
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00d70500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 601); // 0x00df0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 602); // 0x00df0d00
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00d70d01
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00d70600
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00d70503
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 606); // 0x00df0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 607); // 0x00df0d00
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000110, 610); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004013); // 0x00a24013
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 612); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000001); // 0x00a70e01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 615); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 618); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000111, 619); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000010); // 0x00a70610
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 622); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000111, 623); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000010); // 0x00a70d10
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 644); // 0x00af0600
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 645); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 646); // 0x00bf0600
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 647); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 648); // 0x00cf0600
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 649); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 650); // 0x00df0600
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 651); // 0x00df0500
	snd_hda_codec_write(codec, 0x0a, 0, 0x7f0, 0x00000003);
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 669); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 672); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000110, 673); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00a70600
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 677); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000111, 678); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000010); // 0x00a70d10
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 681); // 0x00bf0500
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00b70500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 684); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 685); // 0x00bf0d00
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00b70d01
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00b70600
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00b70503
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 689); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000001, 690); // 0x00bf0d00
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00b70d00
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 693); // 0x00cf0500
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00c70500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 696); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 697); // 0x00cf0d00
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00c70d01
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00c70600
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00c70503
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 701); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 702); // 0x00cf0d00
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 704); // 0x00df0500
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00d70500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 707); // 0x00df0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 708); // 0x00df0d00
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00d70d01
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00d70600
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00d70503
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 712); // 0x00df0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 713); // 0x00df0d00
	snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_CONNECT_SEL, 0x00000000); // 0x01d70100
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000001); // 0x00a70e01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 718); // 0x01df0700
	snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000040); // 0x01d70740
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000000); // 0x00a70e00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00a70d00
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 748); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 751); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 752); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00a70d01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00a70600
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 756); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000001, 757); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00a70d00
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 760); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_STREAM_FORMAT, 0x00000000); // 0x00a20000
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000040, 762); // 0x01df0700
	snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01d70700
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x00000033, 771); // 0x006b2000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00006033); // 0x00636033
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x00000033, 773); // 0x006b0000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00005033); // 0x00635033
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x00000033, 782); // 0x006b2000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00006033); // 0x00636033
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x00000033, 784); // 0x006b0000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00005033); // 0x00635033
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 823); // 0x006f0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 824); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 827); // 0x010f0900
	retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 828); // 0x011f0900
	retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 829); // 0x017f0900
	retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 830); // 0x018f0900
	retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 831); // 0x021f0900
	retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x80000000, 832); // 0x022f0900
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 838); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01070500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 841); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01070503
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 843); // 0x010f0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
	snd_hda_codec_write(codec, 0x24, 0, 0x7f0, 0x00000000);
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 885); // 0x010f0900
	retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 886); // 0x011f0900
	retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 887); // 0x017f0900
	retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 888); // 0x018f0900
	retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 889); // 0x021f0900
	retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x80000000, 890); // 0x022f0900
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 896); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01070500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 899); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01070503
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 901); // 0x010f0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
	snd_hda_codec_write(codec, 0x24, 0, 0x7f0, 0x00000000);
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 955); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01070500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 958); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01070503
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 960); // 0x010f0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x00000033, 969); // 0x006b2000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00006033); // 0x00636033
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x00000033, 971); // 0x006b0000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00005033); // 0x00635033
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x00000033, 980); // 0x006b2000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00006033); // 0x00636033
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x00000033, 982); // 0x006b0000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00005033); // 0x00635033
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1047); // 0x010f0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01070500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1053); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01070503
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1055); // 0x010f0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1058); // 0x010f0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01070500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1064); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01070503
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1066); // 0x010f0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1076); // 0x006f0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1077); // 0x00af0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004031); // 0x00624031
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000000, 0x10134208, 1097); // 0x000f0000
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000000, 0x10134208, 1483); // 0x000f0000
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1491); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1492); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00a70d01
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1496); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01070500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1499); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01070503
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1501); // 0x010f0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000001, 1504); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00a70d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004013); // 0x00a24013
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1507); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00a70d01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000001); // 0x00a70e01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1511); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000010); // 0x00a70610
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000010, 1531); // 0x00af0600
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1532); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1534); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000111, 1535); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000010); // 0x00a70d10
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1538); // 0x00bf0600
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1539); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1540); // 0x00cf0600
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1541); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1542); // 0x00df0600
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1543); // 0x00df0500
	snd_hda_codec_write(codec, 0x0a, 0, 0x7f0, 0x00000003);
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1563); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000110, 1564); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1566); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000010); // 0x00a70610
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1568); // 0x00bf0500
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00b70500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1571); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1572); // 0x00bf0d00
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00b70d01
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00b70600
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00b70503
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1576); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000001, 1577); // 0x00bf0d00
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00b70d00
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1580); // 0x00cf0500
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00c70500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1583); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1584); // 0x00cf0d00
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00c70d01
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00c70600
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00c70503
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1588); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1589); // 0x00cf0d00
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1591); // 0x00df0500
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00d70500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1594); // 0x00df0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1595); // 0x00df0d00
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00d70d01
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00d70600
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00d70503
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1599); // 0x00df0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1600); // 0x00df0d00
	snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_CONNECT_SEL, 0x00000000); // 0x01d70100
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000001); // 0x00a70e01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 1605); // 0x01df0700
	snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000040); // 0x01d70740
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000001); // 0x00171501
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1619); // 0x006f0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1620); // 0x00af0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000000); // 0x00a70e00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00a70d00
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1635); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00a70600
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1637); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_STREAM_FORMAT, 0x00000000); // 0x00a20000
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000040, 1639); // 0x01df0700
	snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01d70700
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1644); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1645); // 0x00af0d00
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1649); // 0x006f0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1650); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000000, 0x10134208, 1653); // 0x000f0000
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1661); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1662); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00a70d01
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000001, 1666); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00a70d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004013); // 0x00a24013
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1669); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00a70d01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000001); // 0x00a70e01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1673); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000010); // 0x00a70610
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000010, 1693); // 0x00af0600
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1694); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1696); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000111, 1697); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000010); // 0x00a70d10
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1700); // 0x00bf0600
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1701); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1702); // 0x00cf0600
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1703); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1704); // 0x00df0600
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1705); // 0x00df0500
	snd_hda_codec_write(codec, 0x0a, 0, 0x7f0, 0x00000003);
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1725); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000110, 1726); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1728); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000010); // 0x00a70610
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1730); // 0x00bf0500
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00b70500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1733); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1734); // 0x00bf0d00
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00b70d01
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00b70600
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00b70503
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1738); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000001, 1739); // 0x00bf0d00
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00b70d00
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1742); // 0x00cf0500
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00c70500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1745); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1746); // 0x00cf0d00
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00c70d01
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00c70600
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00c70503
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1750); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1751); // 0x00cf0d00
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1753); // 0x00df0500
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00d70500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1756); // 0x00df0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1757); // 0x00df0d00
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00d70d01
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00d70600
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00d70503
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1761); // 0x00df0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1762); // 0x00df0d00
	snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_CONNECT_SEL, 0x00000000); // 0x01d70100
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000001); // 0x00a70e01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 1767); // 0x01df0700
	snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000040); // 0x01d70740
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000001); // 0x00171501
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1781); // 0x006f0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1782); // 0x00af0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000000); // 0x00a70e00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00a70d00
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1797); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00a70600
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1799); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_STREAM_FORMAT, 0x00000000); // 0x00a20000
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000040, 1801); // 0x01df0700
	snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01d70700
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503

	codec_dbg(codec, "setup_a1534 end");
	return 0;
}

static int play_a1534 (struct hda_codec *codec) {
        int retval;
        //printk("snd_hda_intel: play_a1534 begin");

	retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000000, 0x10134208, 1); // 0x000f0000
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 9); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 10); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00a70d01
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000001, 14); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00a70d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004013); // 0x00a24013
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 17); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00a70d01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000001); // 0x00a70e01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 21); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000010); // 0x00a70610
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000010, 41); // 0x00af0600
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 42); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 44); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000111, 45); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000010); // 0x00a70d10
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 48); // 0x00bf0600
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 49); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 50); // 0x00cf0600
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 51); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 52); // 0x00df0600
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 53); // 0x00df0500
	snd_hda_codec_write(codec, 0x0a, 0, 0x7f0, 0x00000003);
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 73); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000110, 74); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 76); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000010); // 0x00a70610
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 78); // 0x00bf0500
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00b70500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 81); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 82); // 0x00bf0d00
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00b70d01
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00b70600
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00b70503
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 86); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000001, 87); // 0x00bf0d00
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00b70d00
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 90); // 0x00cf0500
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00c70500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 93); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 94); // 0x00cf0d00
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00c70d01
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00c70600
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00c70503
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 98); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 99); // 0x00cf0d00
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 101); // 0x00df0500
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00d70500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 104); // 0x00df0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 105); // 0x00df0d00
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00d70d01
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00d70600
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00d70503
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 109); // 0x00df0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 110); // 0x00df0d00
	snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_CONNECT_SEL, 0x00000000); // 0x01d70100
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000001); // 0x00a70e01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 115); // 0x01df0700
	snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000040); // 0x01d70740
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000001); // 0x00171501
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 129); // 0x006f0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 130); // 0x00af0500

        //printk("snd_hda_intel: play_a1534 end");
	return 0;
}

#include "patch_cirrus_a1534_pcm.h"

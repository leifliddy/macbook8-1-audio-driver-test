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
	retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000000, 0x80862808, 9); // 0x000f0000
	retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000002, 0x00100000, 10); // 0x000f0002
	retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000000, 0x10134208, 11); // 0x000f0000
	retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000002, 0x00100401, 12); // 0x000f0002
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
	retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e01f5, 153); // 0x009f000a
	retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 154); // 0x009f000b
	retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x80033f33, 155); // 0x009f000d
	retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 156); // 0x009f000e
	retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x0000001c, 157); // 0x009f0200
	retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 158); // 0x009f000f
	snd_hda_codec_write(codec, 0x09, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00970503
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 161); // 0x00af0005
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00046631, 162); // 0x00af0009
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 163); // 0x00af000f
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x000e0060, 164); // 0x00af000a
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000001, 165); // 0x00af000b
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 166); // 0x00af0012
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 167); // 0x00af000d
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00046631, 168); // 0x00af0009
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x000e0060, 170); // 0x00af000a
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000001, 171); // 0x00af000b
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 172); // 0x00af000f
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_STRIPE_CONTROL, 0x00000000, 0x00100000, 173); // 0x00af2400
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 176); // 0x00bf0005
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00046631, 177); // 0x00bf0009
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 178); // 0x00bf000f
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x000e0060, 179); // 0x00bf000a
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000001, 180); // 0x00bf000b
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 181); // 0x00bf0012
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 182); // 0x00bf000d
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00046631, 183); // 0x00bf0009
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00b70500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x000e0060, 185); // 0x00bf000a
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000001, 186); // 0x00bf000b
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 187); // 0x00bf000f
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_STRIPE_CONTROL, 0x00000000, 0x00100000, 188); // 0x00bf2400
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00b70503
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 191); // 0x00cf0005
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00136711, 192); // 0x00cf0009
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 193); // 0x00cf000f
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e0060, 194); // 0x00cf000a
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000001, 195); // 0x00cf000b
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 196); // 0x00cf0012
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 197); // 0x00cf000d
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00136711, 198); // 0x00cf0009
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00c70500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e0060, 200); // 0x00cf000a
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000001, 201); // 0x00cf000b
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 202); // 0x00cf000e
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x0000001f, 203); // 0x00cf0200
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 204); // 0x00cf000f
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00c70503
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 207); // 0x00df0005
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00136711, 208); // 0x00df0009
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 209); // 0x00df000f
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e0060, 210); // 0x00df000a
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000001, 211); // 0x00df000b
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 212); // 0x00df0012
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 213); // 0x00df000d
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00136711, 214); // 0x00df0009
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00d70500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e0060, 216); // 0x00df000a
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000001, 217); // 0x00df000b
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 218); // 0x00df000e
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x00000020, 219); // 0x00df0200
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 220); // 0x00df000f
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00d70503
	retval = snd_hda_codec_read_check(codec, 0x0e, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 223); // 0x00ef0005
	retval = snd_hda_codec_read_check(codec, 0x0e, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00040631, 224); // 0x00ef0009
	retval = snd_hda_codec_read_check(codec, 0x0e, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 225); // 0x00ef000f
	retval = snd_hda_codec_read_check(codec, 0x0e, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x000e07f0, 226); // 0x00ef000a
	retval = snd_hda_codec_read_check(codec, 0x0e, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000005, 227); // 0x00ef000b
	retval = snd_hda_codec_read_check(codec, 0x0e, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 228); // 0x00ef0012
	retval = snd_hda_codec_read_check(codec, 0x0e, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 229); // 0x00ef000d
	retval = snd_hda_codec_read_check(codec, 0x0e, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00040631, 230); // 0x00ef0009
	snd_hda_codec_write(codec, 0x0e, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00e70500
	retval = snd_hda_codec_read_check(codec, 0x0e, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x000e07f0, 232); // 0x00ef000a
	retval = snd_hda_codec_read_check(codec, 0x0e, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000005, 233); // 0x00ef000b
	retval = snd_hda_codec_read_check(codec, 0x0e, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 234); // 0x00ef000f
	retval = snd_hda_codec_read_check(codec, 0x0e, 0, AC_VERB_GET_STRIPE_CONTROL, 0x00000000, 0x00100000, 235); // 0x00ef2400
	snd_hda_codec_write(codec, 0x0e, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00e70503
	retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 238); // 0x00ff0005
	retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_PARAMETERS, 0x00000009, 0x001b0791, 239); // 0x00ff0009
	retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 240); // 0x00ff000f
	retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x000e05f0, 241); // 0x00ff000a
	retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000005, 242); // 0x00ff000b
	retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 243); // 0x00ff0012
	retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 244); // 0x00ff000d
	retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_PARAMETERS, 0x00000009, 0x001b0791, 245); // 0x00ff0009
	snd_hda_codec_write(codec, 0x0f, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00f70500
	retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x000e05f0, 247); // 0x00ff000a
	retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000005, 248); // 0x00ff000b
	retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 249); // 0x00ff000e
	retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x00000022, 250); // 0x00ff0200
	retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 251); // 0x00ff000f
	snd_hda_codec_write(codec, 0x0f, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00f70503
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 254); // 0x010f0005
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040058d, 255); // 0x010f0009
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 256); // 0x010f000f
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_PARAMETERS, 0x00000012, 0x80034242, 257); // 0x010f0012
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 258); // 0x010f000d
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040058d, 259); // 0x010f0009
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01070500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x002b4020, 261); // 0x010f1c00
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x0000001c, 262); // 0x010f000c
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_PARAMETERS, 0x00000012, 0x80034242, 263); // 0x010f0012
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 264); // 0x010f000e
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x00000002, 265); // 0x010f0200
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 266); // 0x010f000f
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 267); // 0x010f0700
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01070700
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01070503
	retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 271); // 0x011f0005
	retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00400581, 272); // 0x011f0009
	retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 273); // 0x011f000f
	retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 274); // 0x011f0012
	retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 275); // 0x011f000d
	retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00400581, 276); // 0x011f0009
	snd_hda_codec_write(codec, 0x11, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01170500
	retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 278); // 0x011f1c00
	retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000054, 279); // 0x011f000c
	retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 280); // 0x011f000e
	retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x00000002, 281); // 0x011f0200
	retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 282); // 0x011f000f
	retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 283); // 0x011f0700
	snd_hda_codec_write(codec, 0x11, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01170700
	snd_hda_codec_write(codec, 0x11, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01170503
	retval = snd_hda_codec_read_check(codec, 0x12, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 287); // 0x012f0005
	retval = snd_hda_codec_read_check(codec, 0x12, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00400501, 288); // 0x012f0009
	retval = snd_hda_codec_read_check(codec, 0x12, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 289); // 0x012f000f
	retval = snd_hda_codec_read_check(codec, 0x12, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 290); // 0x012f0012
	retval = snd_hda_codec_read_check(codec, 0x12, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 291); // 0x012f000d
	retval = snd_hda_codec_read_check(codec, 0x12, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00400501, 292); // 0x012f0009
	snd_hda_codec_write(codec, 0x12, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01270500
	retval = snd_hda_codec_read_check(codec, 0x12, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 294); // 0x012f1c00
	retval = snd_hda_codec_read_check(codec, 0x12, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000050, 295); // 0x012f000c
	retval = snd_hda_codec_read_check(codec, 0x12, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 296); // 0x012f000e
	retval = snd_hda_codec_read_check(codec, 0x12, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x00000003, 297); // 0x012f0200
	retval = snd_hda_codec_read_check(codec, 0x12, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 298); // 0x012f000f
	retval = snd_hda_codec_read_check(codec, 0x12, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 299); // 0x012f0700
	snd_hda_codec_write(codec, 0x12, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01270700
	snd_hda_codec_write(codec, 0x12, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01270503
	retval = snd_hda_codec_read_check(codec, 0x13, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 303); // 0x013f0005
	retval = snd_hda_codec_read_check(codec, 0x13, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00400501, 304); // 0x013f0009
	retval = snd_hda_codec_read_check(codec, 0x13, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 305); // 0x013f000f
	retval = snd_hda_codec_read_check(codec, 0x13, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 306); // 0x013f0012
	retval = snd_hda_codec_read_check(codec, 0x13, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 307); // 0x013f000d
	retval = snd_hda_codec_read_check(codec, 0x13, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00400501, 308); // 0x013f0009
	snd_hda_codec_write(codec, 0x13, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01370500
	retval = snd_hda_codec_read_check(codec, 0x13, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 310); // 0x013f1c00
	retval = snd_hda_codec_read_check(codec, 0x13, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000050, 311); // 0x013f000c
	retval = snd_hda_codec_read_check(codec, 0x13, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 312); // 0x013f000e
	retval = snd_hda_codec_read_check(codec, 0x13, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x00000004, 313); // 0x013f0200
	retval = snd_hda_codec_read_check(codec, 0x13, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 314); // 0x013f000f
	retval = snd_hda_codec_read_check(codec, 0x13, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 315); // 0x013f0700
	snd_hda_codec_write(codec, 0x13, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01370700
	snd_hda_codec_write(codec, 0x13, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01370503
	retval = snd_hda_codec_read_check(codec, 0x14, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 319); // 0x014f0005
	retval = snd_hda_codec_read_check(codec, 0x14, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00400501, 320); // 0x014f0009
	retval = snd_hda_codec_read_check(codec, 0x14, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 321); // 0x014f000f
	retval = snd_hda_codec_read_check(codec, 0x14, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 322); // 0x014f0012
	retval = snd_hda_codec_read_check(codec, 0x14, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 323); // 0x014f000d
	retval = snd_hda_codec_read_check(codec, 0x14, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00400501, 324); // 0x014f0009
	snd_hda_codec_write(codec, 0x14, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01470500
	retval = snd_hda_codec_read_check(codec, 0x14, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 326); // 0x014f1c00
	retval = snd_hda_codec_read_check(codec, 0x14, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000050, 327); // 0x014f000c
	retval = snd_hda_codec_read_check(codec, 0x14, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 328); // 0x014f000e
	retval = snd_hda_codec_read_check(codec, 0x14, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x00000005, 329); // 0x014f0200
	retval = snd_hda_codec_read_check(codec, 0x14, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 330); // 0x014f000f
	retval = snd_hda_codec_read_check(codec, 0x14, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 331); // 0x014f0700
	snd_hda_codec_write(codec, 0x14, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01470700
	snd_hda_codec_write(codec, 0x14, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01470503
	retval = snd_hda_codec_read_check(codec, 0x15, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 335); // 0x015f0005
	retval = snd_hda_codec_read_check(codec, 0x15, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040040b, 336); // 0x015f0009
	retval = snd_hda_codec_read_check(codec, 0x15, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 337); // 0x015f000f
	retval = snd_hda_codec_read_check(codec, 0x15, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 338); // 0x015f0012
	retval = snd_hda_codec_read_check(codec, 0x15, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270300, 339); // 0x015f000d
	retval = snd_hda_codec_read_check(codec, 0x15, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040040b, 340); // 0x015f0009
	snd_hda_codec_write(codec, 0x15, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01570500
	retval = snd_hda_codec_read_check(codec, 0x15, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 342); // 0x015f1c00
	retval = snd_hda_codec_read_check(codec, 0x15, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00001720, 343); // 0x015f000c
	retval = snd_hda_codec_read_check(codec, 0x15, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270300, 344); // 0x015f000d
	retval = snd_hda_codec_read_check(codec, 0x15, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 345); // 0x015f000f
	retval = snd_hda_codec_read_check(codec, 0x15, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 346); // 0x015f0700
	snd_hda_codec_write(codec, 0x15, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01570700
	snd_hda_codec_write(codec, 0x15, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01570503
	retval = snd_hda_codec_read_check(codec, 0x16, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 350); // 0x016f0005
	retval = snd_hda_codec_read_check(codec, 0x16, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040040b, 351); // 0x016f0009
	retval = snd_hda_codec_read_check(codec, 0x16, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 352); // 0x016f000f
	retval = snd_hda_codec_read_check(codec, 0x16, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 353); // 0x016f0012
	retval = snd_hda_codec_read_check(codec, 0x16, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270300, 354); // 0x016f000d
	retval = snd_hda_codec_read_check(codec, 0x16, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040040b, 355); // 0x016f0009
	snd_hda_codec_write(codec, 0x16, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01670500
	retval = snd_hda_codec_read_check(codec, 0x16, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 357); // 0x016f1c00
	retval = snd_hda_codec_read_check(codec, 0x16, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00001720, 358); // 0x016f000c
	retval = snd_hda_codec_read_check(codec, 0x16, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270300, 359); // 0x016f000d
	retval = snd_hda_codec_read_check(codec, 0x16, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 360); // 0x016f000f
	retval = snd_hda_codec_read_check(codec, 0x16, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 361); // 0x016f0700
	snd_hda_codec_write(codec, 0x16, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01670700
	snd_hda_codec_write(codec, 0x16, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01670503
	retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 365); // 0x017f0005
	retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040048b, 366); // 0x017f0009
	retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 367); // 0x017f000f
	retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 368); // 0x017f0012
	retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270300, 369); // 0x017f000d
	retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040048b, 370); // 0x017f0009
	snd_hda_codec_write(codec, 0x17, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01770500
	retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 372); // 0x017f1c00
	retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000024, 373); // 0x017f000c
	retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270300, 374); // 0x017f000d
	retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 375); // 0x017f000f
	retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 376); // 0x017f0700
	snd_hda_codec_write(codec, 0x17, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01770700
	snd_hda_codec_write(codec, 0x17, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01770503
	retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 380); // 0x018f0005
	retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040048a, 381); // 0x018f0009
	retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 382); // 0x018f000f
	retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 383); // 0x018f0012
	retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270300, 384); // 0x018f000d
	retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040048a, 385); // 0x018f0009
	snd_hda_codec_write(codec, 0x18, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01870500
	retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x00ab9030, 387); // 0x018f1c00
	retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00001724, 388); // 0x018f000c
	retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270300, 389); // 0x018f000d
	retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 390); // 0x018f000f
	retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 391); // 0x018f0700
	snd_hda_codec_write(codec, 0x18, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01870700
	snd_hda_codec_write(codec, 0x18, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01870503
	retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 395); // 0x019f0005
	retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040000b, 396); // 0x019f0009
	retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 397); // 0x019f000f
	retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 398); // 0x019f0012
	retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270200, 399); // 0x019f000d
	retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040000b, 400); // 0x019f0009
	retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x90a60100, 401); // 0x019f1c00
	retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000020, 402); // 0x019f000c
	retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270200, 403); // 0x019f000d
	retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 404); // 0x019f0700
	snd_hda_codec_write(codec, 0x19, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01970700
	retval = snd_hda_codec_read_check(codec, 0x1a, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 407); // 0x01af0005
	retval = snd_hda_codec_read_check(codec, 0x1a, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040000b, 408); // 0x01af0009
	retval = snd_hda_codec_read_check(codec, 0x1a, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 409); // 0x01af000f
	retval = snd_hda_codec_read_check(codec, 0x1a, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 410); // 0x01af0012
	retval = snd_hda_codec_read_check(codec, 0x1a, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270200, 411); // 0x01af000d
	retval = snd_hda_codec_read_check(codec, 0x1a, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040000b, 412); // 0x01af0009
	retval = snd_hda_codec_read_check(codec, 0x1a, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 413); // 0x01af1c00
	retval = snd_hda_codec_read_check(codec, 0x1a, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000020, 414); // 0x01af000c
	retval = snd_hda_codec_read_check(codec, 0x1a, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270200, 415); // 0x01af000d
	retval = snd_hda_codec_read_check(codec, 0x1a, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 416); // 0x01af0700
	snd_hda_codec_write(codec, 0x1a, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01a70700
	retval = snd_hda_codec_read_check(codec, 0x1b, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 419); // 0x01bf0005
	retval = snd_hda_codec_read_check(codec, 0x1b, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040000b, 420); // 0x01bf0009
	retval = snd_hda_codec_read_check(codec, 0x1b, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 421); // 0x01bf000f
	retval = snd_hda_codec_read_check(codec, 0x1b, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 422); // 0x01bf0012
	retval = snd_hda_codec_read_check(codec, 0x1b, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270200, 423); // 0x01bf000d
	retval = snd_hda_codec_read_check(codec, 0x1b, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040000b, 424); // 0x01bf0009
	retval = snd_hda_codec_read_check(codec, 0x1b, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 425); // 0x01bf1c00
	retval = snd_hda_codec_read_check(codec, 0x1b, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000020, 426); // 0x01bf000c
	retval = snd_hda_codec_read_check(codec, 0x1b, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270200, 427); // 0x01bf000d
	retval = snd_hda_codec_read_check(codec, 0x1b, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 428); // 0x01bf0700
	snd_hda_codec_write(codec, 0x1b, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01b70700
	retval = snd_hda_codec_read_check(codec, 0x1c, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 431); // 0x01cf0005
	retval = snd_hda_codec_read_check(codec, 0x1c, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040000b, 432); // 0x01cf0009
	retval = snd_hda_codec_read_check(codec, 0x1c, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 433); // 0x01cf000f
	retval = snd_hda_codec_read_check(codec, 0x1c, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 434); // 0x01cf0012
	retval = snd_hda_codec_read_check(codec, 0x1c, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270200, 435); // 0x01cf000d
	retval = snd_hda_codec_read_check(codec, 0x1c, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040000b, 436); // 0x01cf0009
	retval = snd_hda_codec_read_check(codec, 0x1c, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 437); // 0x01cf1c00
	retval = snd_hda_codec_read_check(codec, 0x1c, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000020, 438); // 0x01cf000c
	retval = snd_hda_codec_read_check(codec, 0x1c, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270200, 439); // 0x01cf000d
	retval = snd_hda_codec_read_check(codec, 0x1c, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 440); // 0x01cf0700
	snd_hda_codec_write(codec, 0x1c, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01c70700
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 443); // 0x01df0005
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00406301, 444); // 0x01df0009
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 445); // 0x01df000f
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 446); // 0x01df0012
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 447); // 0x01df000d
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00406301, 448); // 0x01df0009
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x90100110, 449); // 0x01df1c00
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000010, 450); // 0x01df000c
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 451); // 0x01df000e
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x0000000a, 452); // 0x01df0200
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 453); // 0x01df0700
	snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01d70700
	retval = snd_hda_codec_read_check(codec, 0x1e, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 456); // 0x01ef0005
	retval = snd_hda_codec_read_check(codec, 0x1e, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00406301, 457); // 0x01ef0009
	retval = snd_hda_codec_read_check(codec, 0x1e, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 458); // 0x01ef000f
	retval = snd_hda_codec_read_check(codec, 0x1e, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 459); // 0x01ef0012
	retval = snd_hda_codec_read_check(codec, 0x1e, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 460); // 0x01ef000d
	retval = snd_hda_codec_read_check(codec, 0x1e, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00406301, 461); // 0x01ef0009
	retval = snd_hda_codec_read_check(codec, 0x1e, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 462); // 0x01ef1c00
	retval = snd_hda_codec_read_check(codec, 0x1e, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000010, 463); // 0x01ef000c
	retval = snd_hda_codec_read_check(codec, 0x1e, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 464); // 0x01ef000e
	retval = snd_hda_codec_read_check(codec, 0x1e, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x0000000b, 465); // 0x01ef0200
	retval = snd_hda_codec_read_check(codec, 0x1e, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 466); // 0x01ef0700
	snd_hda_codec_write(codec, 0x1e, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01e70700
	retval = snd_hda_codec_read_check(codec, 0x1f, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 469); // 0x01ff0005
	retval = snd_hda_codec_read_check(codec, 0x1f, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00406201, 470); // 0x01ff0009
	retval = snd_hda_codec_read_check(codec, 0x1f, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 471); // 0x01ff000f
	retval = snd_hda_codec_read_check(codec, 0x1f, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 472); // 0x01ff0012
	retval = snd_hda_codec_read_check(codec, 0x1f, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 473); // 0x01ff000d
	retval = snd_hda_codec_read_check(codec, 0x1f, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00406201, 474); // 0x01ff0009
	retval = snd_hda_codec_read_check(codec, 0x1f, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 475); // 0x01ff1c00
	retval = snd_hda_codec_read_check(codec, 0x1f, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000020, 476); // 0x01ff000c
	retval = snd_hda_codec_read_check(codec, 0x1f, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 477); // 0x01ff0700
	snd_hda_codec_write(codec, 0x1f, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01f70700
	retval = snd_hda_codec_read_check(codec, 0x20, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 480); // 0x020f0005
	retval = snd_hda_codec_read_check(codec, 0x20, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00406201, 481); // 0x020f0009
	retval = snd_hda_codec_read_check(codec, 0x20, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 482); // 0x020f000f
	retval = snd_hda_codec_read_check(codec, 0x20, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 483); // 0x020f0012
	retval = snd_hda_codec_read_check(codec, 0x20, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 484); // 0x020f000d
	retval = snd_hda_codec_read_check(codec, 0x20, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00406201, 485); // 0x020f0009
	retval = snd_hda_codec_read_check(codec, 0x20, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 486); // 0x020f1c00
	retval = snd_hda_codec_read_check(codec, 0x20, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000020, 487); // 0x020f000c
	retval = snd_hda_codec_read_check(codec, 0x20, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 488); // 0x020f0700
	snd_hda_codec_write(codec, 0x20, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x02070700
	retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 491); // 0x021f0005
	retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00400381, 492); // 0x021f0009
	retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 493); // 0x021f000f
	retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 494); // 0x021f0012
	retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 495); // 0x021f000d
	retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00400381, 496); // 0x021f0009
	retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 497); // 0x021f1c00
	retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000014, 498); // 0x021f000c
	retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 499); // 0x021f000e
	retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x0000000e, 500); // 0x021f0200
	retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 501); // 0x021f0700
	snd_hda_codec_write(codec, 0x21, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x02170700
	retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 504); // 0x022f0005
	retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00400281, 505); // 0x022f0009
	retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 506); // 0x022f000f
	retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 507); // 0x022f0012
	retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 508); // 0x022f000d
	retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00400281, 509); // 0x022f0009
	retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 510); // 0x022f1c00
	retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000024, 511); // 0x022f000c
	retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 512); // 0x022f0700
	snd_hda_codec_write(codec, 0x22, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x02270700
	retval = snd_hda_codec_read_check(codec, 0x23, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 515); // 0x023f0005
	retval = snd_hda_codec_read_check(codec, 0x23, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00700200, 516); // 0x023f0009
	retval = snd_hda_codec_read_check(codec, 0x23, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 517); // 0x023f000f
	retval = snd_hda_codec_read_check(codec, 0x23, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 518); // 0x023f0012
	retval = snd_hda_codec_read_check(codec, 0x23, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 519); // 0x023f000d
	retval = snd_hda_codec_read_check(codec, 0x23, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00700200, 520); // 0x023f0009
	snd_hda_codec_write(codec, 0x23, 0, AC_VERB_SET_BEEP_CONTROL, 0x00000000); // 0x02370a00
	retval = snd_hda_codec_read_check(codec, 0x24, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 523); // 0x024f0005
	retval = snd_hda_codec_read_check(codec, 0x24, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00f0e2c1, 524); // 0x024f0009
	retval = snd_hda_codec_read_check(codec, 0x24, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 525); // 0x024f000f
	retval = snd_hda_codec_read_check(codec, 0x24, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 526); // 0x024f0012
	retval = snd_hda_codec_read_check(codec, 0x24, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 527); // 0x024f000d
	retval = snd_hda_codec_read_check(codec, 0x24, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00f0e2c1, 528); // 0x024f0009
	retval = snd_hda_codec_read_check(codec, 0x24, 0, AC_VERB_PARAMETERS, 0x00000010, 0x00008000, 529); // 0x024f0010
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	retval = snd_hda_codec_read_check(codec, 0x25, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 1069); // 0x025f0005
	retval = snd_hda_codec_read_check(codec, 0x25, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00000000, 1070); // 0x025f0009
	retval = snd_hda_codec_read_check(codec, 0x25, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 1071); // 0x025f000f
	retval = snd_hda_codec_read_check(codec, 0x25, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x00000000, 1072); // 0x025f000a
	retval = snd_hda_codec_read_check(codec, 0x25, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000000, 1073); // 0x025f000b
	retval = snd_hda_codec_read_check(codec, 0x25, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 1074); // 0x025f0012
	retval = snd_hda_codec_read_check(codec, 0x25, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 1075); // 0x025f000d
	retval = snd_hda_codec_read_check(codec, 0x25, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00000000, 1076); // 0x025f0009
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000004); // 0x00171604
	retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_GET_GPIO_DATA, 0x00000000, 0x00000004, 1104); // 0x001f1500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000006); // 0x00171606
	retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_GET_GPIO_DATA, 0x00000000, 0x00000006, 1107); // 0x001f1500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
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
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 1176); // 0x010f0900
	retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 1177); // 0x011f0900
	retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 1178); // 0x017f0900
	retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 1179); // 0x018f0900
	retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 1180); // 0x021f0900
	retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x80000000, 1181); // 0x022f0900
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
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1229); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01070500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1232); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01070503
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1234); // 0x010f0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_UNSOLICITED_ENABLE, 0x00000083); // 0x01070883
	snd_hda_codec_write(codec, 0x18, 0, AC_VERB_SET_UNSOLICITED_ENABLE, 0x00000009); // 0x01870809
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004031); // 0x00624031
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1261); // 0x006f0500
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00670500
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000030, 1264); // 0x006f0500
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00670600
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00670503
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1267); // 0x006f0500
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x000000b3, 1278); // 0x006b2000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x000060a7); // 0x006360a7
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x000000b3, 1280); // 0x006b0000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x000050a7); // 0x006350a7
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x000000a7, 1282); // 0x006b2000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x000060a7); // 0x006360a7
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x000000a7, 1284); // 0x006b0000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x000050a7); // 0x006350a7
	retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x00000000, 1286); // 0x019b2000
	snd_hda_codec_write(codec, 0x19, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00006000); // 0x01936000
	retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x00000000, 1288); // 0x019b0000
	snd_hda_codec_write(codec, 0x19, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00005000); // 0x01935000
	retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x00000000, 1290); // 0x019b2000
	snd_hda_codec_write(codec, 0x19, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00006000); // 0x01936000
	retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x00000000, 1292); // 0x019b0000
	snd_hda_codec_write(codec, 0x19, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00005000); // 0x01935000
	retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 1294); // 0x019f0700
	snd_hda_codec_write(codec, 0x19, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000020); // 0x01970720
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004031); // 0x00624031
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x000000a7, 1321); // 0x006b2000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x000060b3); // 0x006360b3
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x000000a7, 1323); // 0x006b0000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x000050b3); // 0x006350b3
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x000000b3, 1326); // 0x006b2000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00006033); // 0x00636033
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x000000b3, 1328); // 0x006b0000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00005033); // 0x00635033
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1337); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004013); // 0x00a24013
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1339); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000001); // 0x00a70e01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1342); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1345); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000111, 1346); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00a70600
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1349); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000111, 1350); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000010); // 0x00a70d10
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1371); // 0x00af0600
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1372); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1373); // 0x00bf0600
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1374); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1375); // 0x00cf0600
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1376); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1377); // 0x00df0600
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1378); // 0x00df0500
	snd_hda_codec_write(codec, 0x0a, 0, 0x7f0, 0x00000003);
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1396); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1399); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000110, 1400); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00a70600
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1404); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000111, 1405); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000010); // 0x00a70d10
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1408); // 0x00bf0500
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00b70500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1411); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1412); // 0x00bf0d00
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00b70d01
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00b70600
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00b70503
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1416); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000001, 1417); // 0x00bf0d00
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00b70d00
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1420); // 0x00cf0500
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00c70500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1423); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1424); // 0x00cf0d00
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00c70d01
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00c70600
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00c70503
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1428); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1429); // 0x00cf0d00
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1431); // 0x00df0500
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00d70500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1434); // 0x00df0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1435); // 0x00df0d00
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00d70d01
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00d70600
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00d70503
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1439); // 0x00df0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1440); // 0x00df0d00
	snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_CONNECT_SEL, 0x00000000); // 0x01d70100
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000001); // 0x00a70e01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 1445); // 0x01df0700
	snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000040); // 0x01d70740
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000000); // 0x00a70e00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00a70d00
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1463); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1466); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1467); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00a70d01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00a70600
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1471); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000001, 1472); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00a70d00
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1475); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_STREAM_FORMAT, 0x00000000); // 0x00a20000
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000040, 1477); // 0x01df0700
	snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01d70700
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1480); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004013); // 0x00a24013
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1482); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000001); // 0x00a70e01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1503); // 0x00af0600
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1504); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1505); // 0x00bf0600
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1506); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1507); // 0x00cf0600
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1508); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1509); // 0x00df0600
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1510); // 0x00df0500
	snd_hda_codec_write(codec, 0x0a, 0, 0x7f0, 0x00000003);
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1528); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1531); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000111, 1532); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00a70600
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1535); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000111, 1536); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000010); // 0x00a70d10
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1539); // 0x00bf0500
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00b70500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1542); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1543); // 0x00bf0d00
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00b70d01
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00b70600
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00b70503
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1547); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000001, 1548); // 0x00bf0d00
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00b70d00
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1551); // 0x00cf0500
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00c70500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1554); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1555); // 0x00cf0d00
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00c70d01
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00c70600
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00c70503
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1559); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1560); // 0x00cf0d00
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1562); // 0x00df0500
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00d70500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1565); // 0x00df0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1566); // 0x00df0d00
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00d70d01
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00d70600
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00d70503
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1570); // 0x00df0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1571); // 0x00df0d00
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000110, 1574); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004013); // 0x00a24013
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1576); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000001); // 0x00a70e01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1579); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1582); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000111, 1583); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000010); // 0x00a70610
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1586); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000111, 1587); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000010); // 0x00a70d10
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1608); // 0x00af0600
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1609); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1610); // 0x00bf0600
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1611); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1612); // 0x00cf0600
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1613); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1614); // 0x00df0600
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1615); // 0x00df0500
	snd_hda_codec_write(codec, 0x0a, 0, 0x7f0, 0x00000003);
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1633); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1636); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000110, 1637); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00a70600
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1641); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000111, 1642); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000010); // 0x00a70d10
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1645); // 0x00bf0500
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00b70500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1648); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1649); // 0x00bf0d00
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00b70d01
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00b70600
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00b70503
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1653); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000001, 1654); // 0x00bf0d00
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00b70d00
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1657); // 0x00cf0500
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00c70500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1660); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1661); // 0x00cf0d00
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00c70d01
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00c70600
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00c70503
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1665); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1666); // 0x00cf0d00
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1668); // 0x00df0500
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00d70500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1671); // 0x00df0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1672); // 0x00df0d00
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00d70d01
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00d70600
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00d70503
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1676); // 0x00df0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1677); // 0x00df0d00
	snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_CONNECT_SEL, 0x00000000); // 0x01d70100
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000001); // 0x00a70e01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 1682); // 0x01df0700
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
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1712); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1715); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1716); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00a70d01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00a70600
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1720); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000001, 1721); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00a70d00
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1724); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_STREAM_FORMAT, 0x00000000); // 0x00a20000
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000040, 1726); // 0x01df0700
	snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01d70700
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x00000033, 1735); // 0x006b2000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00006033); // 0x00636033
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x00000033, 1737); // 0x006b0000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00005033); // 0x00635033
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x00000033, 1746); // 0x006b2000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00006033); // 0x00636033
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x00000033, 1748); // 0x006b0000
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
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1787); // 0x006f0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1788); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 1791); // 0x010f0900
	retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 1792); // 0x011f0900
	retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 1793); // 0x017f0900
	retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 1794); // 0x018f0900
	retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 1795); // 0x021f0900
	retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x80000000, 1796); // 0x022f0900
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1802); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01070500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1805); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01070503
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1807); // 0x010f0500
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
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 1849); // 0x010f0900
	retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 1850); // 0x011f0900
	retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 1851); // 0x017f0900
	retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 1852); // 0x018f0900
	retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 1853); // 0x021f0900
	retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x80000000, 1854); // 0x022f0900
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1860); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01070500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1863); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01070503
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1865); // 0x010f0500
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
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1919); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01070500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1922); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01070503
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1924); // 0x010f0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x00000033, 1933); // 0x006b2000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00006033); // 0x00636033
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x00000033, 1935); // 0x006b0000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00005033); // 0x00635033
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x00000033, 1944); // 0x006b2000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00006033); // 0x00636033
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x00000033, 1946); // 0x006b0000
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
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2011); // 0x010f0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01070500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 2017); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01070503
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2019); // 0x010f0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2022); // 0x010f0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01070500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 2028); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01070503
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2030); // 0x010f0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2040); // 0x006f0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2041); // 0x00af0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004031); // 0x00624031
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000000, 0x10134208, 2061); // 0x000f0000
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 2069); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 2070); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00a70d01
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2074); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01070500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 2077); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01070503
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2079); // 0x010f0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000001, 2082); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00a70d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004013); // 0x00a24013
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 2085); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00a70d01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000001); // 0x00a70e01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 2089); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000010); // 0x00a70610
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000010, 2109); // 0x00af0600
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 2110); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2112); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000111, 2113); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000010); // 0x00a70d10
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 2116); // 0x00bf0600
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2117); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 2118); // 0x00cf0600
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2119); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 2120); // 0x00df0600
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2121); // 0x00df0500
	snd_hda_codec_write(codec, 0x0a, 0, 0x7f0, 0x00000003);
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 2141); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000110, 2142); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 2144); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000010); // 0x00a70610
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2146); // 0x00bf0500
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00b70500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 2149); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 2150); // 0x00bf0d00
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00b70d01
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00b70600
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00b70503
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2154); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000001, 2155); // 0x00bf0d00
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00b70d00
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2158); // 0x00cf0500
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00c70500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 2161); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 2162); // 0x00cf0d00
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00c70d01
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00c70600
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00c70503
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2166); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 2167); // 0x00cf0d00
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2169); // 0x00df0500
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00d70500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 2172); // 0x00df0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 2173); // 0x00df0d00
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00d70d01
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00d70600
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00d70503
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2177); // 0x00df0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 2178); // 0x00df0d00
	snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_CONNECT_SEL, 0x00000000); // 0x01d70100
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000001); // 0x00a70e01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 2183); // 0x01df0700
	snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000040); // 0x01d70740
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000001); // 0x00171501
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2197); // 0x006f0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 2198); // 0x00af0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000000); // 0x00a70e00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00a70d00
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 2215); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00a70600
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 2217); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_STREAM_FORMAT, 0x00000000); // 0x00a20000
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000040, 2219); // 0x01df0700
	snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01d70700
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2224); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 2225); // 0x00af0d00
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2229); // 0x006f0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2230); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000000, 0x10134208, 2233); // 0x000f0000
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2243); // 0x010f0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01070500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 2249); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01070503
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2251); // 0x010f0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2261); // 0x006f0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2262); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000000, 0x10134208, 2265); // 0x000f0000
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x00000033, 2271); // 0x006b2000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00006033); // 0x00636033
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x00000033, 2273); // 0x006b0000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00005033); // 0x00635033
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x00000033, 2282); // 0x006b2000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00006033); // 0x00636033
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x00000033, 2284); // 0x006b0000
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
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2323); // 0x010f0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01070500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 2329); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01070503
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2331); // 0x010f0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2341); // 0x006f0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2342); // 0x00af0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 2352); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 2353); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00a70d01
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000001, 2357); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00a70d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004013); // 0x00a24013
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 2360); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00a70d01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000001); // 0x00a70e01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 2364); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000010); // 0x00a70610
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000010, 2384); // 0x00af0600
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 2385); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2387); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000111, 2388); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000010); // 0x00a70d10
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 2391); // 0x00bf0600
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2392); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 2393); // 0x00cf0600
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2394); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 2395); // 0x00df0600
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2396); // 0x00df0500
	snd_hda_codec_write(codec, 0x0a, 0, 0x7f0, 0x00000003);
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 2416); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000110, 2417); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 2419); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000010); // 0x00a70610
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2421); // 0x00bf0500
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00b70500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 2424); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 2425); // 0x00bf0d00
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00b70d01
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00b70600
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00b70503
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2429); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000001, 2430); // 0x00bf0d00
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00b70d00
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2433); // 0x00cf0500
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00c70500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 2436); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 2437); // 0x00cf0d00
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00c70d01
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00c70600
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00c70503
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2441); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 2442); // 0x00cf0d00
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2444); // 0x00df0500
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00d70500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 2447); // 0x00df0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 2448); // 0x00df0d00
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00d70d01
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00d70600
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00d70503
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2452); // 0x00df0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 2453); // 0x00df0d00
	snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_CONNECT_SEL, 0x00000000); // 0x01d70100
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000001); // 0x00a70e01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 2458); // 0x01df0700
	snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000040); // 0x01d70740
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000001); // 0x00171501
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2472); // 0x006f0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 2473); // 0x00af0500

	codec_dbg(codec, "setup_a1534 end");
	return 0;
}

static int play_a1534 (struct hda_codec *codec) {
        int retval;
	codec_dbg(codec, "play_a1534 start");

	retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000000, 0x10134208, 1); // 0x000f0000
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 4); // 0x010f0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01070500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 10); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01070503
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 12); // 0x010f0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 22); // 0x006f0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 23); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000000, 0x10134208, 26); // 0x000f0000
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x00000033, 32); // 0x006b2000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00006033); // 0x00636033
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x00000033, 34); // 0x006b0000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00005033); // 0x00635033
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x00000033, 43); // 0x006b2000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00006033); // 0x00636033
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x00000033, 45); // 0x006b0000
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
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 84); // 0x010f0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01070500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 90); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01070503
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 92); // 0x010f0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 102); // 0x006f0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 103); // 0x00af0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 113); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 114); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00a70d01
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000001, 118); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00a70d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004013); // 0x00a24013
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 121); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00a70d01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000001); // 0x00a70e01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 125); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000010); // 0x00a70610
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000010, 145); // 0x00af0600
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 146); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 148); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000111, 149); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000010); // 0x00a70d10
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 152); // 0x00bf0600
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 153); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 154); // 0x00cf0600
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 155); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 156); // 0x00df0600
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 157); // 0x00df0500
	snd_hda_codec_write(codec, 0x0a, 0, 0x7f0, 0x00000003);
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 177); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000110, 178); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 180); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000010); // 0x00a70610
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 182); // 0x00bf0500
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00b70500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 185); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 186); // 0x00bf0d00
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00b70d01
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00b70600
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00b70503
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 190); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000001, 191); // 0x00bf0d00
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00b70d00
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 194); // 0x00cf0500
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00c70500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 197); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 198); // 0x00cf0d00
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00c70d01
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00c70600
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00c70503
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 202); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 203); // 0x00cf0d00
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 205); // 0x00df0500
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00d70500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 208); // 0x00df0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 209); // 0x00df0d00
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00d70d01
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00d70600
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00d70503
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 213); // 0x00df0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 214); // 0x00df0d00
	snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_CONNECT_SEL, 0x00000000); // 0x01d70100
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000001); // 0x00a70e01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 219); // 0x01df0700
	snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000040); // 0x01d70740
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000001); // 0x00171501
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 233); // 0x006f0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 234); // 0x00af0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000000); // 0x00a70e00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00a70d00
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 249); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00a70600
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 251); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_STREAM_FORMAT, 0x00000000); // 0x00a20000
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000040, 253); // 0x01df0700
	snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01d70700
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 258); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 259); // 0x00af0d00
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 263); // 0x006f0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 264); // 0x00af0500

	codec_dbg(codec, "play_a1534 end");
	return 0;
}

#include "patch_cirrus_a1534_pcm.h"

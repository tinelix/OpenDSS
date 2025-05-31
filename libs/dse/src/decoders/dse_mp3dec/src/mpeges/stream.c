#include <mpeges/stream.h>
#include <stdio.h>

DSE_AUDIO_OUTPUT_INFO dse_audio_output_info;

unsigned char dse_mp3dec_check_sof(const unsigned char* header) {
	// Sync and check ID and Layer fields

	int startOfFrame = (header[0] & 0xFF);
	int fields = (header[1] >> 4) & 0xE;

	return (startOfFrame == 0xFF) && (fields != 0xE);
}

unsigned char dse_mp3dec_get_sri(const unsigned char* header) {
    // SRI -> Sample rate index
	return (header[2] >> 2) & 0x3;
}

unsigned long dse_mp3dec_get_sample_rate(int mpg_ver, int sri) {
    int sample_rate;

    if (mpg_ver == 3) {
        // MPEG-1
        static const int rates[] = { 44100, 48000, 32000 };
        sample_rate = rates[sri];
    } else if (mpg_ver == 2) {
        // MPEG-2
        static const int rates[] = { 22050, 24000, 16000 };
        sample_rate = rates[sri];
    } else {
        // MPEG-2 Extended (MPEG-2.5)
        static const int rates[] = { 11025, 12000, 8000 };
        sample_rate = rates[sri];
    }

    return sample_rate;
}

MP3DEC_LAYERS dse_mp3dec_get_layer(const unsigned char* header) {
    return ((header[1] >> 1) & 0x3) + 1;
}

int dse_mp3dec_parse_frame_header(
    const unsigned char* buf, 
    MP3DEC_FRAME_INFO* info,
    DSE_AUDIO_OUTPUT_INFO* out_info
) {
    unsigned long hdr = GET_BITS(buf, 0, 32);
    int version, layer, sr_idx, bpi, sample_rate, bitrate, ch_mode;
    int brates[] = { 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320 };

    // Check sync word
    if ((hdr & 0xFFFFFC00) != 0xFFF000) return -1;              // Invalid sync word

    // Check MPEG version
    version = (hdr >> 19) & 3;
    if (version == 1) return -2;                                // Invalid MPEG version

    // Check audio coding profile
    layer = (hdr >> 17) & 3;
    if (layer != 1) return -3;                                  // Invalid audio coding profile

    // Check sample rate index
    sr_idx = (hdr >> 10) & 3;
    if (sr_idx >= 3) return -4;                                 // Unsupported sample rate

    sample_rate = dse_mp3dec_get_sample_rate(version, sr_idx);

    // Detect mode (Stereo or Mono)
    ch_mode = (hdr >> 6) & 3;
    info->channels = (ch_mode == 3) ? 1 : 2;                    // JS/Stereo or Mono

    // Calculate frame size
    bpi = (hdr >> 12) & 0xf;                                    // Bitrate index
    if (bpi == 0 || bpi == 15) return -1;                       // ABR/VBR not supported yet
    bitrate = brates[bpi - 1] * 1000;                           // kbps to bps

    info->frame_size = (bitrate * 144) / sample_rate + ((hdr >> 9) & 1); // with padding

    info->sample_rate = sample_rate;
    info->valid = 0;

    out_info->sample_rate = sample_rate;
    out_info->bits_per_sample = 16;
    out_info->channel_mode = info->channels;

    return 0;
}

int dse_mp3dec_decode_frame(
    const unsigned char* input, 
    size_t frame_len, 
    unsigned char* pcm_buf, 
    size_t pcm_buf_size
) {

    MP3DEC_FRAME_INFO frame_info;
    if (!dse_mp3dec_parse_frame_header(
        input, &frame_info, 
        &dse_audio_output_info
    )) 
        return -1;                                              // Invalid frame

    // Memory for storing time spectrum
    float spectrum[576 * 2];                                    // Two side (L+R)

    // IMDCT very rough estimate
    for (int ch = 0; ch < frame_info.channels; ch++) {
        for (int i = 0; i < 576; i++) {
            spectrum[i] = sin(i * PI / 576.0);
        }
    }

    // Convert to PCM buffer
    int output = MIN(pcm_buf_size / 2, 1152 * frame_info.channels);
    for (int i = 0; i < output; i++) {
        pcm_buf[i * 2] = (long)(spectrum[i] * 32767.0) & 0xFF;   // Left channels
        pcm_buf[i * 2 + 1] = (long)(spectrum[i] * 32767.0) >> 8; // Right channels
    }

    return output;
}

/*  MPEG Layer III decoder for OpenDSE (Open Digital Sound Engine)
 *  -----------------------------------------------------------------------------
 *  Published as public domain.
 *
 *  NOTE: The source code was partially written by AI and was later adapted
 *        and improved by the developer, as it simplifies the process of
 *        developing complex media encoding algorithms.
 *
 *        Despite these improvements, the code may contain errors due to the
 *        lack of comprehensive documentation on working with compressed
 *        audio data in the C language.
 */

#include <math/imdct.h>

void dse_mp3dec_imdct_decode(
    double* spectrum_values, 
    double* time_domain_output, int N
) {
    for (int n = 0; n < N; n++) {
        double sum = 0.0;

        for (int k = 0; k < N; k++) {
            double angle = PI * (k + 0.5) * (n + N / 2 + 1) / N;
            sum += spectrum_values[k] * cos(angle);
        }

        time_domain_output[n] = sqrt(2.0 / N) * sum;
    }
}

void dse_mp3dec_imdct_overlap_add(
    double* prev_window, double* curr_window, 
    double* output_signal, int overlap_size
) {
    for (int n = 0; n < overlap_size; n++) {
        double weight_prev = sin(PI * n / overlap_size);
        double weight_curr = sin(PI * (overlap_size - n) / overlap_size);

        output_signal[n] = 
            weight_prev * prev_window[n] + weight_curr * curr_window[n];
    }
}
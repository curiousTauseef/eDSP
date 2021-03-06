/**
 * eDSP, A cross-platform Digital Signal Processing library written in modern C++.
 * Copyright (c) 2018 All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all 
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN 
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * File: variation.hpp
 * Created by: Mohammed Boujemaoui Boulaghmoudi
 * Created at: 15/10/18
 */

#ifndef EDSP_VARIATION_HPP
#define EDSP_VARIATION_HPP

#include <iterator>
#include <cmath>

namespace edsp { namespace feature { inline namespace statistics {

    /**
    * @brief Computes the variation coefficient between the the elements in the range [first, last)
    * and the elements starting in d_first.
    *
    * The variation coefficient is a representation of the similarity between two signals. It is a parameter
    * similar to the flux coefficient:
    *
    * \f[
    *
    *  v = 1 - \frac{\sum_k X_1(k) X_2(K)  }{ \sqrt{ \sum_k X_1(k)} \sqrt{\sum_k X_2(k)}}
    *
    * \f]
    *
    * If the variation is close to 0, the signals amplitudes are really similar. Otherwise,
    * if it is close to 1, they are highly dissimilar.
    *
    * @param first1 Forward iterator defining the begin of the first magnitude spectrum.
    * @param last1 Forward iterator defining the end of the first magnitude spectrum.
    * @param first2 Input iterator defining the beginning of the second magnitude spectrum.
    * @return The estimated spectral variation.
    * @see statistics::flux
    */
    template <typename ForwardIt>
    constexpr auto variation(ForwardIt first1, ForwardIt last1, ForwardIt first2) {
        using value_type = typename std::iterator_traits<ForwardIt>::value_type;
        auto sum_x1      = static_cast<value_type>(0);
        auto sum_x2      = static_cast<value_type>(0);
        auto sum_x12     = static_cast<value_type>(0);
        for (; first1 != last1; ++first1, ++first2) {
            sum_x1 += *first1;
            sum_x2 += *first2;
            sum_x12 += *first1 * *first2;
        }
        return 1 - sum_x2 / (std::sqrt(sum_x1) * std::sqrt(sum_x2));
    }

}}} // namespace edsp::feature::statistics

#endif //EDSP_VARIATION_HPP

/*
 * Copyright (c) 2018, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SENSIRION_SHDLC_H
#define SENSIRION_SHDLC_H

#include "sensirion_arch_config.h"

#define SENSIRION_SHDLC_ERR_NO_DATA -1
#define SENSIRION_SHDLC_ERR_MISSING_START -2
#define SENSIRION_SHDLC_ERR_MISSING_STOP -3
#define SENSIRION_SHDLC_ERR_CRC_MISMATCH -4
#define SENSIRION_SHDLC_ERR_ENCODING_ERROR -5
#define SENSIRION_SHDLC_ERR_TX_INCOMPLETE -6


#if SENSIRION_BIG_ENDIAN
#define be16_to_cpu(s) (s)
#define be32_to_cpu(s) (s)
#define be64_to_cpu(s) (s)
#else
#define be16_to_cpu(s) (((u16)(s) << 8) | (0xff & ((u16)(s)) >> 8))
#define be32_to_cpu(s) (((u32)be16_to_cpu(s) << 16) | \
                        (0xffff & (be16_to_cpu((s) >> 16))))
#define be64_to_cpu(s) (((u64)be32_to_cpu(s) << 32) | \
                        (0xffffffff & ((u64)be32_to_cpu((s) >> 32))))
#endif


struct sensirion_shdlc_rx_header {
    u8 addr;
    u8 cmd;
    u8 state;
    u8 data_len;
};

/**
 * sensirion_shdlc_tx() - transmit an SHDLC frame
 *
 * @addr:       SHDLC recipient address
 * @cmd:        command parameter
 * @data_len:   data length to send
 * @data:       data to send
 * Return:      0 on success, an error code otherwise
 */
s16 sensirion_shdlc_tx(u8 addr, u8 cmd, u8 data_len, const u8 *data);

/**
 * sensirion_shdlc_rx() - receive an SHDLC frame
 *
 * Note that the header and data must be discarded on failure
 *
 * @data_len:   max data length to receive
 * @header:     Memory where the SHDLC header containing the sender address,
 *              command, sensor state and data length is stored
 * @data:       Memeory where received data is stored
 * Return:      0 on success, an error code otherwise
 */
s16 sensirion_shdlc_rx(u8 max_data_len,
                       struct sensirion_shdlc_rx_header *header, u8 *data);

/**
 * sensirion_shdlc_xcv() - transceive (transmit then receive) an SHDLC frame
 *
 * Note that rx_header and rx_data must be discarded on failure
 *
 * @addr:           recipient address
 * @cmd:            parameter
 * @tx_data_len:    data length to send
 * @tx_data:        data to send
 * @rx_header:      Memory where the SHDLC header containing the sender address,
 *                  command, sensor state and data length is stored
 * @rx_data:        Memory where the received data is stored
 * Return:          0 on success, an error code otherwise
 */
s16 sensirion_shdlc_xcv(u8 addr, u8 cmd, u8 tx_data_len, const u8 *tx_data,
                        u8 max_rx_data_len,
                        struct sensirion_shdlc_rx_header *rx_header,
                        u8 *rx_data);

#endif /* SENSIRION_SHDLC_H */

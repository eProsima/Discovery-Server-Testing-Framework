// Copyright 2025 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifdef SECURITY

#include <openssl/opensslv.h>

#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/obj_mac.h>

#include <fastdds/rtps/common/Guid.hpp>

using namespace eprosima::fastdds::rtps;

inline bool mangle_guid(
        const std::string& cert_path,
        const GUID_t& candidate_participant_key,
        GUID_t& adjusted_participant_key)
{
    BIO* bio = BIO_new(BIO_s_file());
    X509* cert;

    if (bio != nullptr)
    {
        if (BIO_read_filename(bio, cert_path.substr(7).c_str()) > 0)
        {
            cert = PEM_read_bio_X509_AUX(bio, NULL, NULL, NULL);
        }
        else
        {
            std::cerr << "OpenSSL library cannot read file "<< cert_path.substr(7) << std::endl;
        }

        BIO_free(bio);
    }
    else
    {
        std::cerr << "OpenSSL library cannot allocate file" << std::endl;
    }

    X509_NAME* cert_sn = X509_get_subject_name(cert);

    unsigned char md[SHA256_DIGEST_LENGTH];
    unsigned int length = 0;

    if (!X509_NAME_digest(cert_sn, EVP_sha256(), md, &length) || length != SHA256_DIGEST_LENGTH)
    {
        std::cerr << "OpenSSL library cannot hash sha256" << std::endl;
        return false;
    }

    adjusted_participant_key.guidPrefix.value[0] = 0x80 | (md[0] >> 1);
    adjusted_participant_key.guidPrefix.value[1] = (md[0] << 7) | (md[1] >> 1);
    adjusted_participant_key.guidPrefix.value[2] = (md[1] << 7) | (md[2] >> 1);
    adjusted_participant_key.guidPrefix.value[3] = (md[2] << 7) | (md[3] >> 1);
    adjusted_participant_key.guidPrefix.value[4] = (md[3] << 7) | (md[4] >> 1);
    adjusted_participant_key.guidPrefix.value[5] = (md[4] << 7) | (md[5] >> 1);

    unsigned char key[16] = {
        candidate_participant_key.guidPrefix.value[0],
        candidate_participant_key.guidPrefix.value[1],
        candidate_participant_key.guidPrefix.value[2],
        candidate_participant_key.guidPrefix.value[3],
        candidate_participant_key.guidPrefix.value[4],
        candidate_participant_key.guidPrefix.value[5],
        candidate_participant_key.guidPrefix.value[6],
        candidate_participant_key.guidPrefix.value[7],
        candidate_participant_key.guidPrefix.value[8],
        candidate_participant_key.guidPrefix.value[9],
        candidate_participant_key.guidPrefix.value[10],
        candidate_participant_key.guidPrefix.value[11],
        candidate_participant_key.entityId.value[0],
        candidate_participant_key.entityId.value[1],
        candidate_participant_key.entityId.value[2],
        candidate_participant_key.entityId.value[3]
    };

    if (!EVP_Digest(&key, 16, md, NULL, EVP_sha256(), NULL))
    {
        std::cerr << "OpenSSL library cannot hash sha256" << std::endl;
        return false;
    }

    adjusted_participant_key.guidPrefix.value[6] = md[0];
    adjusted_participant_key.guidPrefix.value[7] = md[1];
    adjusted_participant_key.guidPrefix.value[8] = md[2];
    adjusted_participant_key.guidPrefix.value[9] = md[3];
    adjusted_participant_key.guidPrefix.value[10] = md[4];
    adjusted_participant_key.guidPrefix.value[11] = md[5];

    adjusted_participant_key.entityId.value[0] = candidate_participant_key.entityId.value[0];
    adjusted_participant_key.entityId.value[1] = candidate_participant_key.entityId.value[1];
    adjusted_participant_key.entityId.value[2] = candidate_participant_key.entityId.value[2];
    adjusted_participant_key.entityId.value[3] = candidate_participant_key.entityId.value[3];

    EVP_cleanup();

    return true;
}

#endif //SECURITY

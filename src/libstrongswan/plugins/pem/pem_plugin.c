/*
 * Copyright (C) 2009 Martin Willi
 * Hochschule fuer Technik Rapperswil
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */

#include "pem_plugin.h"

#include <library.h>

#include "pem_builder.h"
#include "pem_encoder.h"

typedef struct private_pem_plugin_t private_pem_plugin_t;

/**
 * private data of pem_plugin
 */
struct private_pem_plugin_t {

	/**
	 * public functions
	 */
	pem_plugin_t public;
};

METHOD(plugin_t, get_name, char*,
	private_pem_plugin_t *this)
{
	return "pem";
}

METHOD(plugin_t, get_features, int,
	private_pem_plugin_t *this, plugin_feature_t *features[])
{
	static plugin_feature_t f[] = {
		/* private key PEM decoding */
		PLUGIN_REGISTER(PRIVKEY, pem_private_key_load, FALSE),
			PLUGIN_PROVIDE(PRIVKEY, KEY_ANY),
				PLUGIN_DEPENDS(HASHER, HASH_MD5),
		PLUGIN_REGISTER(PRIVKEY, pem_private_key_load, FALSE),
			PLUGIN_PROVIDE(PRIVKEY, KEY_RSA),
				PLUGIN_DEPENDS(HASHER, HASH_MD5),
		PLUGIN_REGISTER(PRIVKEY, pem_private_key_load, FALSE),
			PLUGIN_PROVIDE(PRIVKEY, KEY_ECDSA),
				PLUGIN_DEPENDS(HASHER, HASH_MD5),
		PLUGIN_REGISTER(PRIVKEY, pem_private_key_load, FALSE),
			PLUGIN_PROVIDE(PRIVKEY, KEY_DSA),
				PLUGIN_DEPENDS(HASHER, HASH_MD5),

		/* public key PEM decoding */
		PLUGIN_REGISTER(PUBKEY, pem_public_key_load, FALSE),
			PLUGIN_PROVIDE(PUBKEY, KEY_ANY),
		PLUGIN_REGISTER(PUBKEY, pem_public_key_load, FALSE),
			PLUGIN_PROVIDE(PUBKEY, KEY_RSA),
		PLUGIN_REGISTER(PUBKEY, pem_public_key_load, FALSE),
			PLUGIN_PROVIDE(PUBKEY, KEY_ECDSA),
		PLUGIN_REGISTER(PUBKEY, pem_public_key_load, FALSE),
			PLUGIN_PROVIDE(PUBKEY, KEY_DSA),

		/* certificate PEM decoding */
		PLUGIN_REGISTER(CERT_DECODE, pem_certificate_load, FALSE),
			PLUGIN_PROVIDE(CERT_DECODE, CERT_ANY),
		PLUGIN_REGISTER(CERT_DECODE, pem_certificate_load, FALSE),
			PLUGIN_PROVIDE(CERT_DECODE, CERT_X509),
		PLUGIN_REGISTER(CERT_DECODE, pem_certificate_load, FALSE),
			PLUGIN_PROVIDE(CERT_DECODE, CERT_X509_CRL),
		PLUGIN_REGISTER(CERT_DECODE, pem_certificate_load, FALSE),
			PLUGIN_PROVIDE(CERT_DECODE, CERT_X509_OCSP_REQUEST),
		PLUGIN_REGISTER(CERT_DECODE, pem_certificate_load, FALSE),
			PLUGIN_PROVIDE(CERT_DECODE, CERT_X509_OCSP_RESPONSE),
		PLUGIN_REGISTER(CERT_DECODE, pem_certificate_load, FALSE),
			PLUGIN_PROVIDE(CERT_DECODE, CERT_X509_AC),
		PLUGIN_REGISTER(CERT_DECODE, pem_certificate_load, FALSE),
			PLUGIN_PROVIDE(CERT_DECODE, CERT_PKCS10_REQUEST),
		PLUGIN_REGISTER(CERT_DECODE, pem_certificate_load, FALSE),
			PLUGIN_PROVIDE(CERT_DECODE, CERT_TRUSTED_PUBKEY),
		PLUGIN_REGISTER(CERT_DECODE, pem_certificate_load, FALSE),
			PLUGIN_PROVIDE(CERT_DECODE, CERT_GPG),

		/* pluto specific certificate formats */
		PLUGIN_REGISTER(CERT_DECODE, pem_certificate_load, FALSE),
			PLUGIN_PROVIDE(CERT_DECODE, CERT_PLUTO_CERT),
		PLUGIN_REGISTER(CERT_DECODE, pem_certificate_load, FALSE),
			PLUGIN_PROVIDE(CERT_DECODE, CERT_PLUTO_CRL),
	};
	*features = f;
	return countof(f);
}
METHOD(plugin_t, destroy, void,
	private_pem_plugin_t *this)
{
	free(this);
}

/*
 * see header file
 */
plugin_t *pem_plugin_create()
{
	private_pem_plugin_t *this;

	INIT(this,
		.public = {
			.plugin = {
				.get_name = _get_name,
				.get_features = _get_features,
				.destroy = _destroy,
			},
		},
	);

	/* register PEM encoder */
	lib->encoding->add_encoder(lib->encoding, pem_encoder_encode);

	return &this->public.plugin;
}


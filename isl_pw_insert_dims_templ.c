/*
 * Copyright 2010      INRIA Saclay
 *
 * Use of this software is governed by the MIT license
 *
 * Written by Sven Verdoolaege, INRIA Saclay - Ile-de-France,
 * Parc Club Orsay Universite, ZAC des vignes, 4 rue Jacques Monod,
 * 91893 Orsay, France
 */

__isl_give PW *FN(PW,insert_dims)(__isl_take PW *pw, enum isl_dim_type type,
	unsigned first, unsigned n)
{
	int i;
	isl_size n_piece;
	enum isl_dim_type set_type;
	isl_space *space;

	n_piece = FN(PW,n_piece)(pw);
	if (n_piece < 0)
		return FN(PW,free)(pw);
	if (n == 0 && !isl_space_is_named_or_nested(pw->dim, type))
		return pw;

	set_type = type == isl_dim_in ? isl_dim_set : type;

	space = FN(PW,take_space)(pw);
	space = isl_space_insert_dims(space, type, first, n);
	pw = FN(PW,restore_space)(pw, space);

	pw = FN(PW,cow)(pw);
	if (!pw)
		return NULL;

	for (i = 0; i < n_piece; ++i) {
		pw->p[i].set = isl_set_insert_dims(pw->p[i].set,
							    set_type, first, n);
		if (!pw->p[i].set)
			goto error;
		pw->p[i].FIELD = FN(EL,insert_dims)(pw->p[i].FIELD,
								type, first, n);
		if (!pw->p[i].FIELD)
			goto error;
	}

	return pw;
error:
	FN(PW,free)(pw);
	return NULL;
}

__isl_give PW *FN(PW,add_dims)(__isl_take PW *pw, enum isl_dim_type type,
	unsigned n)
{
	isl_size pos;

	pos = FN(PW,dim)(pw, type);
	if (pos < 0)
		return FN(PW,free)(pw);

	return FN(PW,insert_dims)(pw, type, pos, n);
}

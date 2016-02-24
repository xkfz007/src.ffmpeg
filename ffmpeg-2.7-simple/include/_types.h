/*-
 * Copyright (c) 2002 Mike Barcroft <mike@FreeBSD.org>
 * Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	From: @(#)ansi.h	8.2 (Berkeley) 1/4/94
 *	From: @(#)types.h	8.3 (Berkeley) 1/5/94
 * $FreeBSD: src/sys/i386/include/_types.h,v 1.12 2005/07/02 23:13:31 thompsa Exp $
 */

#ifndef _MACHINE__TYPES_H_
#define	_MACHINE__TYPES_H_

#ifndef _SYS_CDEFS_H_
// #error this file needs sys/cdefs.h as a prerequisite
#endif

#define __NO_STRICT_ALIGNMENT

/*
 * Basic types upon which most other types are built.
 */
typedef	char		__int8_t;
typedef	unsigned char		__uint8_t;
typedef	short			__int16_t;
typedef	unsigned short		__uint16_t;
typedef	int			__int32_t;
typedef	unsigned int		__uint32_t;

#if defined(lint)
/* LONGLONG */
typedef	long long		__int64_t;
/* LONGLONG */
typedef	unsigned long long	__uint64_t;
#elif defined(__GNUCLIKE_ATTRIBUTE_MODE_DI)
typedef	int __attribute__((__mode__(__DI__)))		__int64_t;
typedef	unsigned int __attribute__((__mode__(__DI__)))	__uint64_t;
#else
/* LONGLONG */
typedef	__int64		__int64_t;
/* LONGLONG */
typedef	unsigned __int64	__uint64_t;
#endif

#define uint8_t  __uint8_t
#define uint16_t __uint16_t
#define uint32_t __uint32_t
#define uint64_t __uint64_t

#define int8_t  __int8_t
#define int16_t __int16_t
#define int32_t __int32_t
#define int64_t __int64_t

/*
 * Standard type definitions.
 */
typedef	unsigned long	__clock_t;		/* clock()... */
typedef	unsigned int	__cpumask_t;
typedef	__int32_t	__critical_t;
typedef	double		__double_t;
typedef	double		__float_t;
typedef	__int32_t	__intfptr_t;
typedef	__int64_t	__intmax_t;
typedef	__int32_t	__intptr_t;
typedef	__int32_t	__int_fast8_t;
typedef	__int32_t	__int_fast16_t;
typedef	__int32_t	__int_fast32_t;
typedef	__int64_t	__int_fast64_t;
typedef	__int8_t	__int_least8_t;
typedef	__int16_t	__int_least16_t;
typedef	__int32_t	__int_least32_t;
typedef	__int64_t	__int_least64_t;
typedef	__int32_t	__ptrdiff_t;		/* ptr1 - ptr2 */
typedef	__int32_t	__register_t;
typedef	__int32_t	__segsz_t;		/* segment size (in pages) */
typedef	__uint32_t	__size_t;		/* sizeof() */
typedef	__int32_t	__ssize_t;		/* byte count or error */
typedef	__int32_t	__time_t;		/* time()... */
typedef	__uint32_t	__uintfptr_t;
typedef	__uint64_t	__uintmax_t;
typedef	__uint32_t	__uintptr_t;
typedef	__uint32_t	__uint_fast8_t;
typedef	__uint32_t	__uint_fast16_t;
typedef	__uint32_t	__uint_fast32_t;
typedef	__uint64_t	__uint_fast64_t;
typedef	__uint8_t	__uint_least8_t;
typedef	__uint16_t	__uint_least16_t;
typedef	__uint32_t	__uint_least32_t;
typedef	__uint64_t	__uint_least64_t;
typedef	__uint32_t	__u_register_t;
typedef	__uint32_t	__vm_offset_t;
typedef	__int64_t	__vm_ooffset_t;
#ifdef PAE
typedef	__uint64_t	__vm_paddr_t;
#else
typedef	__uint32_t	__vm_paddr_t;
#endif
typedef	__uint64_t	__vm_pindex_t;
typedef	__uint32_t	__vm_size_t;


typedef	unsigned long	clock_t;		/* clock()... */
typedef	unsigned int	cpumask_t;
typedef	__int32_t	critical_t;
typedef	__int32_t	intfptr_t;
typedef	__int64_t	intmax_t;
typedef	__int32_t	intptr_t;
typedef	__int32_t	int_fast8_t;
typedef	__int32_t	int_fast16_t;
typedef	__int32_t	int_fast32_t;
typedef	__int64_t	int_fast64_t;
typedef	__int8_t	int_least8_t;
typedef	__int16_t	int_least16_t;
typedef	__int32_t	int_least32_t;
typedef	__int64_t	int_least64_t;
typedef	__int32_t	ptrdiff_t;		/* ptr1 - ptr2 */
typedef	__int32_t	register_t;
typedef	__int32_t	segsz_t;		/* segment size (in pages) */
typedef	__uint32_t	size_t;		/* sizeof() */
typedef	__int32_t	ssize_t;		/* byte count or error */
//typedef	__int32_t	time_t;		/* time()... */
typedef	__uint32_t	uintfptr_t;
typedef	__uint64_t	uintmax_t;
typedef	__uint32_t	uintptr_t;
typedef	__uint32_t	uint_fast8_t;
typedef	__uint32_t	uint_fast16_t;
typedef	__uint32_t	uint_fast32_t;
typedef	__uint64_t	uint_fast64_t;
typedef	__uint8_t	uint_least8_t;
typedef	__uint16_t	uint_least16_t;
typedef	__uint32_t	uint_least32_t;
typedef	__uint64_t	uint_least64_t;
typedef	__uint32_t	u_register_t;
typedef	__uint32_t	vm_offset_t;
typedef	__int64_t	vm_ooffset_t;

#define inline __inline

#ifndef __SYMBIAN32__
/*
 * Unusual type definitions.
 */
#ifdef __GNUCLIKE_BUILTIN_VARARGS
typedef __builtin_va_list	__va_list;	/* internally known to gcc */
#else
typedef	char *			__va_list;
#endif /* __GNUCLIKE_BUILTIN_VARARGS */
#if defined(__GNUC_VA_LIST_COMPATIBILITY) && !defined(__GNUC_VA_LIST) \
    && !defined(__NO_GNUC_VA_LIST)
#define __GNUC_VA_LIST
typedef __va_list		__gnuc_va_list;	/* compatibility w/GNU headers*/
#endif

//__SYMBIAN32__
#endif

#endif /* !_MACHINE__TYPES_H_ */
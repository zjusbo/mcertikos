#ifndef _KERN_STDARG_H_
#define _KERN_STDARG_H_

#ifdef _KERN_

typedef __builtin_va_list       va_list;

#define va_start(ap, last)	__builtin_va_start((ap), (last))

#define va_arg(ap, type)	__builtin_va_arg((ap), type)

#define va_end(ap)		__builtin_va_end(ap)

#endif /* _KERN_ */

#endif /* !_KERN_STDARG_H_ */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_type.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmazier <kmazier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 04:27:45 by kmazier           #+#    #+#             */
/*   Updated: 2020/12/03 01:08:00 by kmazier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_print_arg_int(va_list *ap, t_flags *flags, size_t *length)
{
	size_t		len;
	long int	nbr;
	int			negative;

	negative = 0;
	nbr = (long int)va_arg(*ap, int);
	len = ft_nblen(nbr);
	if (nbr == 0 && flags->amount_set == 1 && flags->amount_show == 0)
		len--;
	*length += len;
	if (nbr < 0)
	{
		ft_putchar_fd('-', 1);
		nbr *= -1;
		negative = 1;
	}
	ft_print_flags(flags, 0, len, length);
	ft_print_nb_flags(flags, &len, length, negative);
	if (len)
		ft_putnbr_fd(nbr, 1);
	ft_print_flags(flags, 1, len, length);
}

void	ft_print_arg_uint(va_list *ap, t_flags *flags, size_t *length)
{
	size_t 			len;
	unsigned int	nbr;
	
	nbr = (unsigned int)va_arg(*ap, unsigned int);
	*length += (len = ft_unblen(nbr, (flags->type == 'x' || flags->type == 'X')));
	ft_print_flags(flags, 0, len, length);
	ft_print_nb_flags(flags, &len, length, 0);
	if (flags->type == 'x' || flags->type == 'X')
		ft_print_hex(nbr, flags->type == 'X');
	else
		ft_putunbr_fd(nbr, 1);
	ft_print_flags(flags, 1, len, length);
}

void	ft_print_arg_string(va_list *ap, t_flags *flags, size_t *length)
{
	size_t		len;
	const char 	*str;

	str = (const char*)va_arg(*ap, const char*);
	len = ft_strlen(str);
	if (flags->amount_set && len > (size_t)flags->amount_show)
		len = (size_t)flags->amount_show > len ? len + 1 : (size_t)flags->amount_show;
	*length += len;
	ft_print_flags(flags, 0, len, length);
	if (str)
		write(1, str, len);
	ft_print_flags(flags, 1, len, length);
}

void	ft_print_arg_character(va_list *ap, t_flags *flags, size_t *length)
{
	size_t			len;
	unsigned char 	c;

	c = (unsigned char)va_arg(*ap, int);
	*length += (len = 1);
	ft_print_flags(flags, 0, len, length);
	ft_putchar_fd(c, 1);
	ft_print_flags(flags, 1, len, length);
}

void	ft_print_arg_pointer(va_list *ap, t_flags *flags, size_t *length)
{
	size_t			len;
	unsigned long	n;

	n = (unsigned long)va_arg(*ap, unsigned long);
	*length += (len = (ft_unblen(n, 1) + 2));
	ft_print_flags(flags, 0, len, length);
	ft_putstr_fd("0x", 1);
	len -= 2;
	ft_print_nb_flags(flags, &len, length, 0);
	len += 2;
	ft_print_hex(n, 0);
	ft_print_flags(flags, 1, len, length);
}
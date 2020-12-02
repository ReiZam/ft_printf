/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmazier <kmazier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 03:21:33 by kmazier           #+#    #+#             */
/*   Updated: 2020/12/02 22:10:32 by kmazier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		ft_is_conversions(char c)
{
	return (c == 'd' || c == 'i' || c == 'x' || c == 'X' || c == 's' || c == 'c' || c == 'u' || c == 'p');
}

int		ft_check_flags(t_flags *flags)
{
	return (flags->type != '\0');
}

int	ft_parse_nb(char *str, va_list *ap, size_t *offset)
{
	int		result;
	size_t 	i;

	i = 0;
	result = 0;
	if (str[i] == '*')
	{
		*offset += 1;
		return ((int) va_arg(*ap, int));
	}
	while (str[i])
	{
		if (str[i] >= '0' && str[i] <= '9')
			result = result * 10 + (int)(str[i] - 48);
		else
			break ;
		i++;
	}
	*offset += i;
	return (result);
}
#include <stdio.h>

t_flags	*ft_parse_flags(char *str, va_list *ap, size_t *f_len)
{
	t_flags	*flags;
	size_t	i;

	i = 1;
	if (!(flags = (t_flags*)malloc(sizeof(t_flags))))
		return (NULL);
	flags->type = 0;
	flags->amount_show = 0;
	flags->left_zero = 0;
	flags->amount_set = 0;
	flags->lzero_set = 0;
	flags->spaces = 0;
	flags->spaces_set = 0;
	if (str[i] == '*' || (str[i] >= '1' && str[i] <= '9'))
	{
		flags->spaces = ft_parse_nb(str + i, ap, &i);
		flags->spaces_set = 1;
	}
	while (str[i])
	{
		if (str[i] == '-' && flags->spaces_set == 0)
		{
			flags->spaces = (-ft_parse_nb(str + i + 1, ap, &i));
			flags->spaces_set = 1;
		}
		else if (str[i] == '0' && flags->lzero_set == 0)
		{
			flags->left_zero = ft_parse_nb(str + i + 1, ap, &i);
			flags->lzero_set = 1;
		}
		else if (str[i] == '.' && flags->amount_set == 0)
		{
			flags->amount_show = ft_parse_nb(str + i + 1, ap, &i);
			flags->amount_set = 1;
		}
		else if (ft_is_conversions(str[i]))
		{
			flags->type = str[i];
			*f_len = i;
			if (flags->spaces_set == 1 && flags->amount_set == 1 && flags->amount_show > 0)
				if (flags->amount_show < flags->spaces)
					flags->spaces = flags->spaces - flags->amount_show;
			if (flags->spaces > 0)
				flags->spaces *= -1;
			return (flags);
		}
		else
			break ;
		i++;
	}
	free(flags);
	return (NULL);
}

size_t	ft_print_arg(va_list *ap, char *str, size_t *length)
{
	t_flags	*flags;
	size_t	f_len;

	if (!(flags = ft_parse_flags(str, ap, &f_len)) || !ft_check_flags(flags))
		return (0);
	if (flags->type == 'd' || flags->type == 'i')
		ft_print_arg_int(ap, flags, length);
	else if (flags->type == 'x' || flags->type == 'X' || flags->type == 'u')
		ft_print_arg_uint(ap, flags, length);
	else if (flags->type == 's')
		ft_print_arg_string(ap, flags, length);
	else if (flags->type == 'c')
		ft_print_arg_character(ap, flags, length);
	else if (flags->type == 'p')
		ft_print_arg_pointer(ap, flags, length);
	free(flags);
	flags = NULL;
	return (f_len);
}

int		ft_printf(const char *str, ...)
{
	size_t	i;
	size_t	temp;
	size_t	length;
	va_list	ap;
	
	i = -1;
	length = 0;
	va_start(ap, str);
	while (str[++i])
	{
		if (str[i] == '%' && str[i + 1])
		{
			if (str[i + 1] == '%')
				i++;
			else if ((temp = ft_print_arg(&ap, (char*)str + i, &length)))
			{
				i += temp;
				continue;
			}
		}
		ft_putchar_fd(str[i], 1);
		length++;
	}
	va_end(ap);
	return (length);
}

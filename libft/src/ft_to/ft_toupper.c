/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 15:57:16 by sinawara          #+#    #+#             */
/*   Updated: 2024/10/07 15:57:17 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
		c -= 32;
	return (c);
}
/*
int main() {
    char ch;

    printf("Enter a character: ");
    scanf("%c", &ch);

    int upper = ft_toupper(ch);
    printf("Uppercase character: %c\n", upper);

    return 0;
}
*/

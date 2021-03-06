#include "../includes/cub3d.h"

void	put_bmp_header(unsigned const int *headers, int fd)
{
	int	n;

	n = 0;
	ft_putstr_fd("BM", fd);
	while (n < 6)
	{
		ft_putchar_fd(headers[n] & 0x000000FF, fd);
		ft_putchar_fd((headers[n] & 0x0000FF00) >> 8, fd);
		ft_putchar_fd((headers[n] & 0x00FF0000) >> 16, fd);
		ft_putchar_fd((headers[n] & (unsigned int) 0xFF000000) >> 24, fd);
		n++;
	}
	ft_putchar_fd(1, fd);
	ft_putchar_fd(0, fd);
	ft_putchar_fd(24, fd);
	ft_putchar_fd(0, fd);
	n++;
	while (n < 13)
	{
		ft_putchar_fd(headers[n] & 0x000000FF, fd);
		ft_putchar_fd((headers[n] & 0x0000FF00) >> 8, fd);
		ft_putchar_fd((headers[n] & 0x00FF0000) >> 16, fd);
		ft_putchar_fd((headers[n] & (unsigned int) 0xFF000000) >> 24, fd);
		n++;
	}
}

static void	put_extrabytes(int extrabytes, int fd)
{
	int	n;

	n = 0;
	while (n < extrabytes)
	{
		ft_putchar_fd(0, fd);
		n++;
	}
}

void	put_bmp_pixels(t_win *win, int fd, int extrabytes)
{
	int	y;
	int	x;
	int	z;

	y = win->height;
	while (y > 0)
	{
		x = 0;
		z = (win->line_length) * (y - 1);
		while (x < win->width)
		{
			ft_putchar_fd(win->addr[z], fd);
			ft_putchar_fd(win->addr[z + 1], fd);
			ft_putchar_fd(win->addr[z + 2], fd);
			z = z + 4;
			x++;
		}
		if (extrabytes)
			put_extrabytes(extrabytes, fd);
		y--;
	}
}

int	screenshot(t_win *win)
{
	unsigned int	headers[13];
	int				fd;
	int				extrabytes;

	extrabytes = 4 - ((win->width * 3) % 4);
	if (extrabytes == 4)
		extrabytes = 0;
	headers[0] = ((win->width * 3) + extrabytes) * win->height + 54;
	headers[1] = 0;
	headers[2] = 54;
	headers[3] = 40;
	headers[4] = win->width;
	headers[5] = win->height;
	headers[7] = 0;
	headers[8] = win->height * win->width * 4;
	headers[9] = 0;
	headers[10] = 0;
	headers[11] = 0;
	headers[12] = 0;
	fd = open("screen.bmp", O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU | S_IROTH);
	put_bmp_header(headers, fd);
	put_bmp_pixels(win, fd, extrabytes);
	close(fd);
	return (1);
}

#include <mlx.h>
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct  s_bool {
	int		w;
	int		s;
	int		a;
	int		d;
	int		l;
	int		r;
}				t_bool;

typedef struct  s_point {
	double x;
	double y;
}				t_point;

typedef struct	s_image {
	void		*ptr;
	void		*addr;
	int         bits_per_pixel;
	int         line_length;
	int         endian;
	int			length;
	int			h;
}				t_image;

typedef struct s_texture
{
	t_image img;
	int		h;
	int		w;
} t_texture;


typedef struct  s_data {
	void    	*mlx;
	void    	*mlx_w;
	void        *img;
	void        *addr;
	int         bits_per_pixel;
	int         line_length;
	int         endian;
	t_point		pos;
	t_point		dir;
	t_point		pla;
	t_texture	texture;
	t_bool		mi_ban;
	int			color;
}               t_data;


// t_image w_img;

#define width 640
#define height 480
#define texWidth 64
#define texHeight 64
#define mapWidth 24
#define mapHeight 24

int worldMap[mapWidth][mapHeight]=
{
 {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};
void	ft_render(t_data *img);

// int		close_w(int keycode, t_data *data)
// {
// 	if (keycode == 53)
// 		exit(0);
// 	return (0);
// }

void            my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char    *dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

int            color(t_data *data, int x, int y)
{
	char    *dst;
	int color;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	color = *(unsigned int*)dst;
	return (color);
}

// int     ft_sqrt(int nb)
// {
// 	return (nb * nb);
// }


int		create_rgb(int r, int g, int b)
{
	return(r << 16 | g << 8 | b);
}

int		key_hook(t_data *data)
{
	// printf("%d\n", keycode);
	
	double moveSpeed = 0.915 * 0.0085 * 10;
	double rotSpeed = 0.915 * 0.0085 * 10;
	if (data->mi_ban.w) //w
	{
		if(worldMap[(int)(data->pos.x + data->dir.x * moveSpeed)][(int)(data->pos.y)] == false) data->pos.x += data->dir.x * moveSpeed;
      	if(worldMap[(int)(data->pos.x)][(int)(data->pos.y + data->dir.y * moveSpeed)] == false) data->pos.y += data->dir.y * moveSpeed;	// data->pos.x++;
	}
	if (data->mi_ban.s) //s
	{
		if(worldMap[(int)(data->pos.x - data->dir.x * moveSpeed)][(int)(data->pos.y)] == false) data->pos.x -= data->dir.x * moveSpeed;
    	if(worldMap[(int)(data->pos.x)][(int)(data->pos.y - data->dir.y * moveSpeed)] == false) data->pos.y -= data->dir.y * moveSpeed;
		// data->pos.x++;
	}
	// if(keycode == 1) //s
	// {
	// 	if(worldMap[int(data->pos.x - dirX * moveSpeed)][int(posY)] == false) posX -= dirX * moveSpeed;
    //   	if(worldMap[int(posX)][int(posY - dirY * moveSpeed)] == false) posY -= dirY * moveSpeed;
	// }
	if (data->mi_ban.d) //d
	{
      double oldDirX = data->dir.x;
      data->dir.x = data->dir.x * cos(-rotSpeed) - data->dir.y * sin(-rotSpeed);
      data->dir.y = oldDirX * sin(-rotSpeed) + data->dir.y * cos(-rotSpeed);
      double oldPlaneX = data->pla.x;
      data->pla.x = data->pla.x * cos(-rotSpeed) - data->pla.y * sin(-rotSpeed);
      data->pla.y = oldPlaneX * sin(-rotSpeed) + data->pla.y * cos(-rotSpeed);
	}
	if (data->mi_ban.a) //a
	{
      double oldDirX = data->dir.x;
      data->dir.x = data->dir.x * cos(rotSpeed) - data->dir.y * sin(rotSpeed);
      data->dir.y = oldDirX * sin(rotSpeed) + data->dir.y * cos(rotSpeed);
      double oldPlaneX = data->pla.x;
      data->pla.x = data->pla.x * cos(rotSpeed) - data->pla.y * sin(rotSpeed);
      data->pla.y = oldPlaneX * sin(rotSpeed) + data->pla.y * cos(rotSpeed);
	}
	// ft_render(data);
	// printf("x --- %f\n", data->pos.x);
		// printf("key --- %d\n", keycode);
	return (0);
}

// int		mouse_hook(int keycode, int x, int y, t_data *data)
// {
// 	printf("%d\n", keycode);
// 	printf("X - %d, Y - %d\n", x, y);
// 	return (0);
// }


void	verLine(int x, int start, int end, t_data data)
{ 
	int y;

	y = start;
	while(y < end){
		my_mlx_pixel_put(&data, x, y, data.color);
		y++;
	}	
}

void	ft_render(t_data *img)
{
	// w_img.h = texHeight;
	// w_img.length = texWidth;
	for(int x = 0; x < width; x++)
	{
	  //calculate ray position and direction
	  double cameraX = 2 * x / (double)width - 1; //x-coordinate in camera space
	  double rayDirX = img->dir.x + img->pla.x * cameraX;
	  double rayDirY = img->dir.y + img->pla.y * cameraX;
	  //which box of the map we're in
	  int mapX = (int)(img->pos.x);
	  int mapY = (int)(img->pos.y);

	  //length of ray from current position to next x or y-side
	  double sideDistX;
	  double sideDistY;

	   //length of ray from one x or y-side to next x or y-side
		double deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
      	double deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));
    	double perpWallDist;

	  //what direction to step in x or y-direction (either +1 or -1)
	  int stepX;
	  int stepY;

	  int hit = 0; //was there a wall hit?
	  int side; //was a NS or a EW wall hit?
	  //calculate step and initial sideDist
	  if (rayDirX < 0)
      {
        stepX = -1;
        sideDistX = (img->pos.x - mapX) * deltaDistX;
      }
      else
      {
        stepX = 1;
        sideDistX = (mapX + 1.0 - img->pos.x) * deltaDistX;
      }
      if (rayDirY < 0)
      {
        stepY = -1;
        sideDistY = (img->pos.y - mapY) * deltaDistY;
      }
      else
      {
        stepY = 1;
        sideDistY = (mapY + 1.0 - img->pos.y) * deltaDistY;
      }
	  //perform DDA
	  while (hit == 0)
	  {
		//jump to next map square, OR in x-direction, OR in y-direction
		if(sideDistX < sideDistY)
		{
		  sideDistX += deltaDistX;
		  mapX += stepX;
		  side = 0;
		}
		else
		{
		  sideDistY += deltaDistY;
		  mapY += stepY;
		  side = 1;
		}
		//Check if ray has hit a wall
		if(worldMap[mapX][mapY] > 0) hit = 1;
	  }
	  //Calculate distance projected on camera direction (Euclidean distance will give fisheye effect!)
	  if(side == 0) perpWallDist = (mapX - img->pos.x + (1 - stepX) / 2) / rayDirX;
	  else          perpWallDist = (mapY - img->pos.y + (1 - stepY) / 2) / rayDirY;

	  //Calculate height of line to draw on screen
	  int lineHeight = (int)(height / perpWallDist);

	  //calculate lowest and highest pixel to fill in current stripe
      int drawStart = -lineHeight / 2 + height / 2;
      if(drawStart < 0) drawStart = 0;
      int drawEnd = lineHeight / 2 + height / 2;
      if(drawEnd >= height) drawEnd = height - 1;

	int texNum = worldMap[mapX][mapY] - 1; //1 subtracted from it so that texture 0 can be used!

		double wallX; //where exactly the wall was hit
      if(side == 0) wallX = img->pos.y + perpWallDist * rayDirY;
      else          wallX = img->pos.x + perpWallDist * rayDirX;
      wallX -= floor((wallX));

      //x coordinate on the texture
      int texX = (int)(wallX * (double)(texWidth));
      if(side == 0 && rayDirX > 0) texX = texWidth - texX - 1;
      if(side == 1 && rayDirY < 0) texX = texWidth - texX - 1;

      // TODO: an integer-only bresenham or DDA like algorithm could make the texture coordinate stepping faster
      // How much to increase the texture coordinate per screen pixel
      double step = 1.0 * texHeight / lineHeight;
      // Starting texture coordinate
      double texPos = (drawStart - height / 2 + lineHeight / 2) * step;
	  int col;
      for(int y = drawStart; y < drawEnd; y++)
      {
        // Cast the texture coordinate to integer, and mask with (texHeight - 1) in case of overflow
        int texY = (int)texPos & (texHeight - 1);
        texPos += step;
		col = color(img, texX, texY);
       // Uint32 color = texture[texNum][texHeight * texY + texX];
        //make color darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
        // if(side == 1) col = (col >> 1) & 8355711;
        //buffer[y][x] = color;
      }


	  //choose wall color
	  int colors;
	  switch(worldMap[mapX][mapY])
	  {
		case 1:  colors = create_rgb(0, 255, 0);   break; //red
		case 2:  colors = create_rgb(0, 255, 0);  break; //green
		case 3:  colors = create_rgb(0, 0, 255);   break; //blue
		case 4:  colors = create_rgb(255, 255, 255);  break; //white
		default: colors = create_rgb(0, 255, 255); break; //yellow
	  }

	  //give x and y sides different brightness
	  if(side == 1 && worldMap[mapX][mapY] != 1) {colors = colors / 2;}
		img->color = colors;
	
	//draw the pixels of the stripe as a vertical line
	 	verLine(x, drawStart, drawEnd, *img);
	}
}

void	ft_clear(t_data *data)
{
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			my_mlx_pixel_put(data, j, i, create_rgb(0, 0, 0));
		}
	}
}

int	 	frame(t_data *data)
{
	ft_clear(data);
	key_hook(data);
	ft_render(data);
	mlx_put_image_to_window(data->mlx, data->mlx_w, data->img, 0, 0);
	mlx_put_image_to_window(data->mlx, data->mlx_w, data->texture.img.ptr, width / 2, 0);
	mlx_do_sync(data->mlx);
	return (1);
}

void	ft_init(t_bool *mi_ban)
{
	mi_ban->a = 0;
	mi_ban->w = 0;
	mi_ban->d = 0;
	mi_ban->s = 0;
	mi_ban->l = 0;
	mi_ban->r = 0;
}

int  ft_move(int keycode, t_data *data)
{
	if (keycode == 53)
		exit(0);
	if (keycode == 13)
		data->mi_ban.w = 1;
	if (keycode == 1)
		data->mi_ban.s = 1;
	if (keycode == 2)
		data->mi_ban.d = 1;
	if (keycode == 0)
		data->mi_ban.a = 1;
	// printf("move %d\n", keycode);
	return (1);
}

int  ft_stop(int keycode, t_data *data)
{
	if (keycode == 13)
		data->mi_ban.w = 0;
	if (keycode == 1)
		data->mi_ban.s = 0;
	if (keycode == 2)
		data->mi_ban.d = 0;
	if (keycode == 0)
		data->mi_ban.a = 0;
	// printf("stop %d\n", keycode);
	return (1);
}

int     main(void)
{
	t_data  img;
	img.pos.x = 20.0;
	img.pos.y = 11.5; 
  	img.dir.x = -1.0;
	img.dir.y = 0.0;
  	img.pla.x = 0.0;
	img.pla.y = 0.66;

	// t_bool mi_ban;
	ft_init(&img.mi_ban);


	img.mlx = mlx_init();
	img.mlx_w = mlx_new_window(img.mlx, width, height, "Cub3D");
	img.img = mlx_new_image(img.mlx, width, height);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);

	img.texture.img.ptr = mlx_xpm_file_to_image(img.mlx, "./images/wall.xpm", &img.texture.w, &img.texture.h);
	img.texture.img.addr = mlx_get_data_addr(img.texture.img.ptr, &img.texture.img.bits_per_pixel, &img.texture.img.line_length, &img.texture.img.endian);

	// mlx_key_hook(img.mlx_w, key_hook, &img);
	mlx_hook(img.mlx_w, 2, 1L<<0, ft_move, &img);
	mlx_hook(img.mlx_w, 3, 1L<<1, ft_stop, &img);
	mlx_loop_hook(img.mlx, frame, &img);
	mlx_loop(img.mlx);
	// mlx_hook(img.mlx_w, 2, 1L<<0, close_w, &img);

	return (0);
}
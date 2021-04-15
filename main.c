#include <mlx.h>
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct  s_point {
	double x;
	double y;
}				t_point;

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
	int			color;
}               t_data;

#define width 640
#define height 480
#define texWidth 64
#define texHeight 64
#define mapWidth 24
#define mapHeight 24

int worldMap[mapWidth][mapHeight]=
{
  {8,8,8,8,8,8,8,8,8,8,8,4,4,6,4,4,6,4,6,4,4,4,6,4},
  {8,0,0,0,0,0,0,0,0,0,8,4,0,0,0,0,0,0,0,0,0,0,0,4},
  {8,0,3,3,0,0,0,0,0,8,8,4,0,0,0,0,0,0,0,0,0,0,0,6},
  {8,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6},
  {8,0,3,3,0,0,0,0,0,8,8,4,0,0,0,0,0,0,0,0,0,0,0,4},
  {8,0,0,0,0,0,0,0,0,0,8,4,0,0,0,0,0,6,6,6,0,6,4,6},
  {8,8,8,8,0,8,8,8,8,8,8,4,4,4,4,4,4,6,0,0,0,0,0,6},
  {7,7,7,7,0,7,7,7,7,0,8,0,8,0,8,0,8,4,0,4,0,6,0,6},
  {7,7,0,0,0,0,0,0,7,8,0,8,0,8,0,8,8,6,0,0,0,0,0,6},
  {7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,6,0,0,0,0,0,4},
  {7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,6,0,6,0,6,0,6},
  {7,7,0,0,0,0,0,0,7,8,0,8,0,8,0,8,8,6,4,6,0,6,6,6},
  {7,7,7,7,0,7,7,7,7,8,8,4,0,6,8,4,8,3,3,3,0,3,3,3},
  {2,2,2,2,0,2,2,2,2,4,6,4,0,0,6,0,6,3,0,0,0,0,0,3},
  {2,2,0,0,0,0,0,2,2,4,0,0,0,0,0,0,4,3,0,0,0,0,0,3},
  {2,0,0,0,0,0,0,0,2,4,0,0,0,0,0,0,4,3,0,0,0,0,0,3},
  {1,0,0,0,0,0,0,0,1,4,4,4,4,4,6,0,6,3,3,0,0,0,3,3},
  {2,0,0,0,0,0,0,0,2,2,2,1,2,2,2,6,6,0,0,5,0,5,0,5},
  {2,2,0,0,0,0,0,2,2,2,0,0,0,2,2,0,5,0,5,0,0,0,5,5},
  {2,0,0,0,0,0,0,0,2,0,0,0,0,0,2,5,0,5,0,5,0,5,0,5},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5},
  {2,0,0,0,0,0,0,0,2,0,0,0,0,0,2,5,0,5,0,5,0,5,0,5},
  {2,2,0,0,0,0,0,2,2,2,0,0,0,2,2,0,5,0,5,0,0,0,5,5},
  {2,2,2,2,1,2,2,2,2,2,2,1,2,2,2,5,5,5,5,5,5,5,5,5}
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

// int     ft_sqrt(int nb)
// {
// 	return (nb * nb);
// }

int		create_rgb(int r, int g, int b)
{
	return(r << 16 | g << 8 | b);
}

int		main(void);

int		key_hook(int keycode, t_data *data)
{
	// printf("%d\n", keycode);
	if (keycode == 53)
		exit(0);
	double moveSpeed = 0.915 * 0.0085 * 10;
	double rotSpeed = 0.915 * 0.0085 * 10;
	if (keycode == 13) //w
	{
		if(worldMap[(int)(data->pos.x + data->dir.x * moveSpeed)][(int)(data->pos.y)] == false) data->pos.x += data->dir.x * moveSpeed;
      	if(worldMap[(int)(data->pos.x)][(int)(data->pos.y + data->dir.y * moveSpeed)] == false) data->pos.y += data->dir.y * moveSpeed;	// data->pos.x++;
	}
	if (keycode == 1) //s
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
	if (keycode == 2) //d
	{
      double oldDirX = data->dir.x;
      data->dir.x = data->dir.x * cos(-rotSpeed) - data->dir.y * sin(-rotSpeed);
      data->dir.y = oldDirX * sin(-rotSpeed) + data->dir.y * cos(-rotSpeed);
      double oldPlaneX = data->pla.x;
      data->pla.x = data->pla.x * cos(-rotSpeed) - data->pla.y * sin(-rotSpeed);
      data->pla.y = oldPlaneX * sin(-rotSpeed) + data->pla.y * cos(-rotSpeed);
	}
	if (keycode == 0) //a
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

	  //choose wall color
	  int color;
	  switch(worldMap[mapX][mapY])
	  {
		case 1:  color = create_rgb(236, 252, 3);    break; //red
		case 2:  color = create_rgb(0, 255, 0);  break; //green
		case 3:  color = create_rgb(0, 0, 255);   break; //blue
		case 4:  color = create_rgb(255, 255, 255);  break; //white
		default: color = create_rgb(0, 255, 255); break; //yellow
	  }

	  //give x and y sides different brightness
	  if(side == 1) {color = color / 2;}
		img->color = color;
	
	//draw the pixels of the stripe as a vertical line
	 	verLine(x, drawStart, drawEnd, *img);
	}
}

int	 	frame(t_data *data)
{
	ft_render(data);
	mlx_put_image_to_window(data->mlx, data->mlx_w, data->img, 0, 0);
	mlx_do_sync(data->mlx);
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

	img.mlx = mlx_init();
	img.mlx_w = mlx_new_window(img.mlx, width, height, "Cub3D");
	img.img = mlx_new_image(img.mlx, width, height);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);

	// int x;
	// int y;
	// // size_t color;
	// // time_t t;

	// x = 0;
	// // srand(time(&t));
	// // color = rand();
	// int img_l;
	// int img_w;
	// char *relativ_path = "./tim.xpm";
	// // img.img = mlx_new_image(img.mlx, 500, 500);
	// img.img = mlx_xpm_file_to_image(img.mlx, relativ_path, &img_w, &img_l);
	// img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);
	// // while (x <= 500)
	// // {
	// // 	y = 0;
	// // 	while (y <= 500)
	// // 	{
	// // 		if ((ft_sqrt(x - 250) + ft_sqrt(y - 250) <= ft_sqrt(100)))
	// // 			my_mlx_pixel_put(&img, x, y, create_trgb(rand(), rand(), rand(), rand()));
	// // 		y++;
	// // 	}
	// // 	x++;
	// // }
	// mlx_put_image_to_window(img.mlx, img.mlx_w, img.img, 0, 0);
	// mlx_hook(img.mlx_w, 2, 1L<<6, close_w, &img);
	// mlx_mouse_hook(img.mlx_w, mouse_hook, &img);
	// mlx_loop_hook(img.mlx, create_trgb, &img);
	// mlx_key_hook(img.mlx_w, close_w, &img);
	mlx_key_hook(img.mlx_w, key_hook, &img);
	mlx_loop_hook(img.mlx, frame, &img);
	mlx_loop(img.mlx);
	// mlx_hook(img.mlx_w, 2, 1L<<0, close_w, &img);

	return (0);
}
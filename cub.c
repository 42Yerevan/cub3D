#include <mlx.h>
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct s_sprite
{
  double x;
  double y;
  int texture;
}               t_sprite;

#define numSprites 1

t_sprite sprite[numSprites] =
{
  {2, 3, 2}
};

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
  t_image     img;
	t_point		pos;
	t_point		dir;
	t_point		pla;
	t_texture	texture;
	t_texture	barrel;
	t_texture	redWall;
	t_bool		mi_ban;
	int			color;
}               t_data;


// t_image w_img;

#define width 1200
#define height 900
#define texWidth 64
#define texHeight 64
#define mapWidth 10
#define mapHeight 10

int worldMap[mapWidth][mapHeight] =
{
  {1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,2,2,2,2,0,0,0,1},
  {1,0,2,2,2,2,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1}
};



//1D Zbuffer
double ZBuffer[width];

//arrays used to sort the sprites
int spriteOrder[numSprites];
double spriteDistance[numSprites];

//function used to sort the sprites
// void sortSprites(int* order, double* dist, int 	);

void sortSprites(int* order, double* dist, int amount)
{
	int		swap;
	double	swapD;

	for(int i = 0; i < amount; i++)
	{
		for(int j = i + 1; j < amount; j++)
		{
			if (dist[i] < dist[j])
			{
				swapD = dist[i];
				dist[i] = dist[j];
				dist[j] = swapD;
				swap = order[i];
				order[i] = order[j];
				order[j] = swap;
			}
		}
	}
}

void	ft_render(t_data *img);

void            my_mlx_pixel_put(t_image *data, int x, int y, int color)
{
	char    *dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

int            color(t_image *data, int x, int y)
{
    return *(int *)(data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8)));
}

int		create_rgb(int r, int g, int b)
{
	return(r << 16 | g << 8 | b);
}

void ft_normalize(double x, double y)
{
  double mod;

	mod = sqrt(x * x + y * y);
	if (mod < 0.0001)
	{
		x = 0;
	  y = 0;
		return ;
	}
	x /= mod;
	y /= mod;
}


int		key_hook(t_data *data)
{
	double moveSpeed = 0.915 * 0.0085 * 10;
	double rotSpeed = 0.915 * 0.0085 * 10;

	double moveX = 1;
	double moveY = 1;

	if (data->mi_ban.l)
		moveY = -1;
	else if (data->mi_ban.r)
		moveY = 1;

	if (data->mi_ban.w && !data->mi_ban.s)
  	{
		ft_normalize(moveX, moveY);
    	moveX *= 0.0085;
    	moveY *= 0.0085;
  	}
	else
  	{
		ft_normalize(moveX, moveY);
    	moveX *= 0.0065;
    	moveY *= 0.0065;
  	}

	if (data->mi_ban.w)
	{
		if(worldMap[(int)(data->pos.x + data->dir.x * moveSpeed)][(int)(data->pos.y)] == false) data->pos.x += data->dir.x * moveSpeed;
      	if(worldMap[(int)(data->pos.x)][(int)(data->pos.y + data->dir.y * moveSpeed)] == false) data->pos.y += data->dir.y * moveSpeed;	// data->pos.x++;
	}
	if (data->mi_ban.s)
	{
		if(worldMap[(int)(data->pos.x - data->dir.x * moveSpeed)][(int)(data->pos.y)] == false) data->pos.x -= data->dir.x * moveSpeed;
    	if(worldMap[(int)(data->pos.x)][(int)(data->pos.y - data->dir.y * moveSpeed)] == false) data->pos.y -= data->dir.y * moveSpeed;
		// data->pos.x++;
	}
	if (data->mi_ban.l)
	{
      double oldDirX = data->dir.x;
      data->dir.x = data->dir.x * cos(-rotSpeed) - data->dir.y * sin(-rotSpeed);
      data->dir.y = oldDirX * sin(-rotSpeed) + data->dir.y * cos(-rotSpeed);
      double oldPlaneX = data->pla.x;
      data->pla.x = data->pla.x * cos(-rotSpeed) - data->pla.y * sin(-rotSpeed);
      data->pla.y = oldPlaneX * sin(-rotSpeed) + data->pla.y * cos(-rotSpeed);
	}
	if (data->mi_ban.r)
	{
      double oldDirX = data->dir.x;
      data->dir.x = data->dir.x * cos(rotSpeed) - data->dir.y * sin(rotSpeed);
      data->dir.y = oldDirX * sin(rotSpeed) + data->dir.y * cos(rotSpeed);
      double oldPlaneX = data->pla.x;
      data->pla.x = data->pla.x * cos(rotSpeed) - data->pla.y * sin(rotSpeed);
      data->pla.y = oldPlaneX * sin(rotSpeed) + data->pla.y * cos(rotSpeed);
	}
	if (data->mi_ban.a)
	{
    	if(!worldMap[(int)(data->pos.x - data->dir.y * moveSpeed)][(int)(data->pos.y)])
      		data->pos.x -= data->dir.y * moveSpeed;
    	if(!worldMap[(int)(data->pos.x)][(int)(data->pos.y + data->dir.x * moveSpeed)])
      		data->pos.y += data->dir.x * moveSpeed;
	}
	if (data->mi_ban.d)
	{
   		if(!worldMap[(int)(data->pos.x + data->dir.y * moveSpeed)][(int)(data->pos.y)])
      		data->pos.x += data->dir.y * moveSpeed;
    	if(!worldMap[(int)(data->pos.x)][(int)(data->pos.y - data->dir.x * moveSpeed)])
      		data->pos.y -= data->dir.x * moveSpeed;
	}
	return (0);
}

void	verLine(int x, int start, int end, t_data data)
{ 
	int y;

	y = start;
	while(y < end){
		my_mlx_pixel_put(&data.img, x, y, data.color);
		y++;
	}	
}

void	ft_render(t_data *img)
{
	for(int x = 0; x < width; x++)
	{
	  double cameraX = 2 * x / (double)width - 1;
	  double rayDirX = img->dir.x + img->pla.x * cameraX;
	  double rayDirY = img->dir.y + img->pla.y * cameraX;
	  int mapX = (int)(img->pos.x);
	  int mapY = (int)(img->pos.y);
	  double sideDistX;
	  double sideDistY;
    double deltaDistX = fabs(1 / rayDirX);
    double deltaDistY = fabs(1 / rayDirY);
    double perpWallDist;

	  int stepX;
	  int stepY;

	  int hit = 0;
	  int side;

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

	  while (hit == 0)
	  {
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
		  if(worldMap[mapX][mapY] > 0 && worldMap[mapX][mapY] < 2) hit = 1;
	  }

	  if(!side) 
      perpWallDist = (mapX - img->pos.x + (1 - stepX) / 2) / rayDirX;
    else
      perpWallDist = (mapY - img->pos.y + (1 - stepY) / 2) / rayDirY;

	  int lineHeight = (int)(height / 2 / perpWallDist);

    int drawStart = -lineHeight / 2 + height / 2;
    if(drawStart < 0) drawStart = 0;
    int drawEnd = lineHeight / 2 + height / 2;
    if(drawEnd >= height) drawEnd = height - 1;

	  double wallX;
    if(!side) 
      wallX = img->pos.y + perpWallDist * rayDirY;
    else
      wallX = img->pos.x + perpWallDist * rayDirX;
    wallX -= floor((wallX));

    int texX = (int)(wallX * (double)(texWidth));

    double step = 1.0 * texHeight / lineHeight;

    double texPos = (drawStart - height / 2 + lineHeight / 2) * step;
	  int col;
    int redWall;

    for(int y = 0; y < drawStart; y++)
    {
      img->color = create_rgb(52, 174, 235);
      my_mlx_pixel_put(&img->img,x, y, img->color);
    }

    for(int y = drawEnd; y < height; y++)
    {
      my_mlx_pixel_put(&img->img,x, y, create_rgb(52, 235, 97));
    }

    for(int y = drawStart; y < drawEnd; y++)
    {
      int texY = (int)texPos & (texHeight - 1);
      texPos += step;
      if(side)
      {
        if(stepY > 0)
          col = color(&img->texture.img, texX, texY);
        else
          col = color(&img->texture.img, texX, texY);
    }
    else
    {
      if(stepX > 0)
        col = color(&img->texture.img, texX, texY);
      else
        col = color(&img->texture.img, texX, texY);
    }   
    redWall = color(&img->redWall.img, texX, texY);
    int colors;
    switch(worldMap[mapX][mapY])
    {
      case 1:  colors = redWall;   break; //red
      case 2:  colors = redWall;  break; //green
      case 3:  colors = redWall;   break; //blue
      case 4:  colors = redWall;  break; //white
      default: colors = redWall; break; //yellow
    }
    if(side == 1 && worldMap[mapX][mapY] != 1) 
      {colors = colors / 2;}
    img->color = colors;
    my_mlx_pixel_put(&img->img, x, y, img->color);
    }	
	  ZBuffer[x] = perpWallDist;
	}

    for(int i = 0; i < numSprites; i++)
    {
      spriteOrder[i] = i;
      spriteDistance[i] = ((img->pos.x - sprite[i].x) * (img->pos.x - sprite[i].x) + (img->pos.y - sprite[i].y) * (img->pos.y - sprite[i].y)); //sqrt not taken, unneeded
    }
    sortSprites(spriteOrder, spriteDistance, numSprites);

    //after sorting the sprites, do the projection and draw them
    for(int i = 0; i < numSprites; i++)
    {
      //translate sprite position to relative to camera
      double spriteX = sprite[spriteOrder[i]].x - img->pos.x;
      double spriteY = sprite[spriteOrder[i]].y - img->pos.y;

      double invDet = 1.0 / (img->pla.x * img->dir.y - img->dir.x * img->pla.y); //required for correct matrix multiplication

      double transformX = invDet * (img->dir.y * spriteX - img->dir.x * spriteY);
      double transformY = invDet * (-img->pla.y * spriteX + img->pla.x * spriteY); //this is actually the depth inside the screen, that what Z is in 3D

      int spriteScreenX = (int)((width / 2) * (1 + transformX / transformY));

    //   //calculate height of the sprite on screen
      int spriteHeight = abs((int)(height / (transformY))); //using 'transformY' instead of the real distance prevents fisheye
    //   //calculate lowest and highest pixel to fill in current stripe
      int drawStartY = -spriteHeight / 2 + height / 2;
      if(drawStartY < 0) drawStartY = 0;
      int drawEndY = spriteHeight / 2 + height / 2;
      if(drawEndY >= height) drawEndY = height - 1;

    //   //calculate width of the sprite
      int spriteWidth = abs( (int) (height / (transformY)));
      int drawStartX = -spriteWidth / 2 + spriteScreenX;
      if(drawStartX < 0) drawStartX = 0;
      int drawEndX = spriteWidth / 2 + spriteScreenX;
      if(drawEndX >= width) drawEndX = width - 1;

    // //   loop through every vertical stripe of the sprite on screen
      for(int stripe = drawStartX; stripe < drawEndX; stripe++)
      {
        int texX = (int)(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * texWidth / spriteWidth) / 256;
        if(transformY > 0 && stripe > 0 && stripe < width && transformY < ZBuffer[stripe])
        {
          for(int y = drawStartY; y < drawEndY; y++) //for every pixel of the current stripe
          {
            int d = (y) * 256 - height * 128 + spriteHeight * 128; //256 and 128 factors to avoid floats
            int texY = ((d * texHeight) / spriteHeight) / 256;
			      int	col = color(&img->barrel.img, texX, texY);
            if((col & 0x00FFFFFF))
              my_mlx_pixel_put(&img->img, stripe, y, col); //paint pixel if it isn't black, black is the invisible color
		   }
        }
      }
    }
}

void	ft_clear(t_data *data)
{
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			my_mlx_pixel_put(&data->img, j, i, create_rgb(0, 0, 0));
		}
	}
}

int	 	frame(t_data *data)
{
	ft_clear(data);
	key_hook(data);
	ft_render(data);
	mlx_put_image_to_window(data->mlx, data->mlx_w, data->img.ptr, 0, 0);
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

int close_win(void)
{
    system("killall afplay");
    exit(0);
    return 0;
}
int  ft_move(int keycode, t_data *data)
{
	if (keycode == 53)
		close_win();
	if (keycode == 13)
		data->mi_ban.w = 1;
	if (keycode == 1)
		data->mi_ban.s = 1;
	if (keycode == 2)
		data->mi_ban.d = 1;
	if (keycode == 0)
		data->mi_ban.a = 1;
	if (keycode == 123)
		data->mi_ban.r = 1;
	if (keycode == 124)
		data->mi_ban.l = 1;
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
	if (keycode == 123)
		data->mi_ban.r = 0;
	if (keycode == 124)
		data->mi_ban.l = 0;
	// printf("stop %d\n", keycode);
	return (1);
}
int     main(void)
{
	t_data  img;
	img.pos.x = 2;
	img.pos.y = 3; 
  img.dir.x = -1.0;
	img.dir.y = 0.0;
  img.pla.x = 0.0;
	img.pla.y = 0.66;

	// t_bool mi_ban;
	ft_init(&img.mi_ban);


	img.mlx = mlx_init();
	img.mlx_w = mlx_new_window(img.mlx, width, height, "Cub3D");
	img.img.ptr = mlx_new_image(img.mlx, width, height);
	img.img.addr = mlx_get_data_addr(img.img.ptr, &img.img.bits_per_pixel, &img.img.line_length, &img.img.endian);
	img.texture.img.ptr = mlx_xpm_file_to_image(img.mlx, "./images/wall.xpm", &img.texture.w, &img.texture.h);
	img.texture.img.addr = mlx_get_data_addr(img.texture.img.ptr, &img.texture.img.bits_per_pixel, &img.texture.img.line_length, &img.texture.img.endian);
  img.barrel.img.ptr = mlx_xpm_file_to_image(img.mlx, "./images/barrel.xpm", &img.barrel.w, &img.barrel.h);
	img.barrel.img.addr = mlx_get_data_addr(img.barrel.img.ptr, &img.barrel.img.bits_per_pixel, &img.barrel.img.line_length, &img.barrel.img.endian);
	img.redWall.img.ptr = mlx_xpm_file_to_image(img.mlx, "./images/redbrick.xpm", &img.redWall.w, &img.redWall.h);
	img.redWall.img.addr = mlx_get_data_addr(img.redWall.img.ptr, &img.redWall.img.bits_per_pixel, &img.redWall.img.line_length, &img.barrel.img.endian);
  system("afplay music.mp3 &");
  mlx_hook(img.mlx_w, 17, 1L<<17, close_win, 0);
	mlx_hook(img.mlx_w, 2, 1L<<0, ft_move, &img);
	mlx_hook(img.mlx_w, 3, 1L<<1, ft_stop, &img);
	mlx_loop_hook(img.mlx, frame, &img);
  mlx_loop(img.mlx);
	return (0);
}
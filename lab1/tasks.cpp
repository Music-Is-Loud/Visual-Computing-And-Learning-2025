#include <random>

#include <spdlog/spdlog.h>

#include "Labs/1-Drawing2D/tasks.h"

using VCX::Labs::Common::ImageRGB;

namespace VCX::Labs::Drawing2D {
    /******************* 1.Image Dithering *****************/
    void DitheringThreshold(
        ImageRGB &       output,
        ImageRGB const & input) {
        for (std::size_t x = 0; x < input.GetSizeX(); ++x)
            for (std::size_t y = 0; y < input.GetSizeY(); ++y) {
                glm::vec3 color = input.At(x, y);
                output.At(x, y) = {
                    color.r > 0.5 ? 1 : 0,
                    color.g > 0.5 ? 1 : 0,
                    color.b > 0.5 ? 1 : 0,
                };
            }
    }

    void DitheringRandomUniform(
        ImageRGB &       output,
        ImageRGB const & input) {
        // your code here:
        std::mt19937 generator(std::random_device{}()); // 随机数生成器
        std::uniform_real_distribution<double> dist(-0.5, 0.5); // 生成 -0.5 到 0.5 之间的浮点数
        double r;
        for (std::size_t x = 0; x < input.GetSizeX(); ++x)
            for (std::size_t y = 0; y < input.GetSizeY(); ++y)
            {
                glm::vec3 color = input.At(x, y);
                r=dist(generator);
                color.r += r;
                color.g += r;
                color.b += r;
                output.At(x, y) = {
                    color.r > 0.5 ? 1 : 0,
                    color.g > 0.5 ? 1 : 0,
                    color.b > 0.5 ? 1 : 0,
                };
            }
    }

    void DitheringRandomBlueNoise(
        ImageRGB &       output,
        ImageRGB const & input,
        ImageRGB const & noise) {
        // your code here:
        for (std::size_t x = 0; x < input.GetSizeX(); ++x)
            for (std::size_t y = 0; y < input.GetSizeY(); ++y)
            {
                glm::vec3 color=input.At(x,y);
                glm::vec3 noise_color=noise.At(x%noise.GetSizeX(),y%noise.GetSizeY());
                color.r+=noise_color.r-0.5;
                color.g+=noise_color.g-0.5;
                color.b+=noise_color.b-0.5;//-0.5从0到1变成-0.5到0.5
                output.At(x, y) = {
                    color.r > 0.5 ? 1 : 0,
                    color.g > 0.5 ? 1 : 0,
                    color.b > 0.5 ? 1 : 0,
                };
            }

    }

    void DitheringOrdered(
        ImageRGB &       output,
        ImageRGB const & input) {
        // your code here:
        int m[3][3]={{6,8,4},{1,0,3},{5,2,7}};
        //double gray;
        //double* out_pixel;
        for (std::size_t x = 0; x < input.GetSizeX(); ++x)
            for (std::size_t y = 0; y < input.GetSizeY(); ++y)
            {
                glm::vec3 color=input.At(x,y);
                //gray=0.3*color.r+0.6*color.g+0.1*color.blue;
                for(std::size_t xx=0;xx<3;xx++)
                {
                    for(std::size_t yy=0;yy<3;yy++)
                    {
                        output.At(3*x+xx, 3*y+yy) = {
                        color.r > float(m[xx][yy]/9.0)?1:0,
                        color.g > float(m[xx][yy]/9.0)?1:0,
                        color.b > float(m[xx][yy]/9.0)?1:0};
                    }
                }
            }

    }

    void DitheringErrorDiffuse(
        ImageRGB &       output,
        ImageRGB const & input) {
        // your code here:
        //double error[3];
        ImageRGB buffer = input;
       
        for (std::size_t x = 1; x < input.GetSizeX()-1; ++x)
            for (std::size_t y = 1; y < input.GetSizeY()-1; ++y)
            {
                glm::vec3 color=buffer.At(x,y);

                glm::vec3 color_to_be={
                    color.r > 0.5 ? 1 : 0,
                    color.g > 0.5 ? 1 : 0,
                    color.b > 0.5 ? 1 : 0,
                };
                output.At(x,y)=color_to_be;
                glm::vec3 error=color-color_to_be;
                buffer.At(x, y+1) = glm::vec3(buffer.At(x, y+1)) + error * (7.0f/16.0f);
                buffer.At(x+1, y-1) = glm::vec3(buffer.At(x+1, y-1)) + error * (3.0f/16.0f);
                buffer.At(x+1, y) = glm::vec3(buffer.At(x+1, y)) + error * (5.0f/16.0f);
                buffer.At(x+1, y+1) = glm::vec3(buffer.At(x+1, y+1)) + error * (1.0f/16.0f);
                
                
            }

    }

    /******************* 2.Image Filtering *****************/
    void Blur(
        ImageRGB &       output,
        ImageRGB const & input) {
        // your code here:
        double kernel[3][3]={{1.0/9,1.0/9,1.0/9},{1.0/9,1.0/9,1.0/9},{1.0/9,1.0/9,1.0/9}};
        glm::vec3 color;
        for (std::size_t x = 1; x < input.GetSizeX()-1; ++x)
            for (std::size_t y = 1; y < input.GetSizeY()-1; ++y)
            {
                color={0,0,0};
                for(std::size_t xx=0;xx<3;xx++)
                {
                    for(std::size_t yy=0;yy<3;yy++)
                    {
                        color+=input.At(x+xx-1,y+yy-1)*float(kernel[xx][yy]);
                    }
                }
                output.At(x,y)=color;
            }

    }

    void Edge(
        ImageRGB &       output,
        ImageRGB const & input) {
        // your code here:
        double kernel1[3][3]={{-1,0,1},{-2,0,2},{-1,0,1}};
        double kernel2[3][3]={{1,2,1},{0,0,0},{-1,-2,-1}};
        glm::vec3 color1,color2;
        for (std::size_t x = 1; x < input.GetSizeX()-1; x++)
            for (std::size_t y = 1; y < input.GetSizeY()-1; y++)
            {
                color1={0,0,0};color2={0,0,0};
                for(std::size_t xx=0;xx<3;xx++)
                {
                    for(std::size_t yy=0;yy<3;yy++)
                    {
                        color1+=(input.At(x+xx-1,y+yy-1)*float(kernel1[xx][yy]));
                        color2+=(input.At(x+xx-1,y+yy-1)*float(kernel2[xx][yy])); 
                    }
                }
                output.At(x,y)=sqrt(color1*color1+color2*color2);
            }

    }

    /******************* 3. Image Inpainting *****************/
    void Inpainting(
        ImageRGB &         output,
        ImageRGB const &   inputBack,
        ImageRGB const &   inputFront,
        const glm::ivec2 & offset) {
        output             = inputBack;
        std::size_t width  = inputFront.GetSizeX();
        std::size_t height = inputFront.GetSizeY();
        glm::vec3 * g      = new glm::vec3[width * height];
        memset(g, 0, sizeof(glm::vec3) * width * height);
        // set boundary condition
        for (std::size_t y = 0; y < height; ++y) {
            // set boundary for (0, y), your code: g[y * width] = ?
            g[y*width]=inputBack.At(offset.x,offset.y+y)-inputFront.At(0,y);
            // set boundary for (width - 1, y), your code: g[y * width + width - 1] = ?
            g[y*width+width-1]=inputBack.At(offset.x+width-1,offset.y+y)-inputFront.At(width-1,y);
        }
        for (std::size_t x = 0; x < width; ++x) {
            // set boundary for (x, 0), your code: g[x] = ?
            g[x]=inputBack.At(offset.x+x,offset.y)-inputFront.At(x,0);
            // set boundary for (x, height - 1), your code: g[(height - 1) * width + x] = ?
            g[(height-1)*width+x]=inputBack.At(offset.x+x,offset.y+height-1)-inputFront.At(x,height-1);
        }

        // Jacobi iteration, solve Ag = b
        for (int iter = 0; iter < 8000; ++iter) {
            for (std::size_t y = 1; y < height - 1; ++y)
                for (std::size_t x = 1; x < width - 1; ++x) {
                    g[y * width + x] = (g[(y - 1) * width + x] + g[(y + 1) * width + x] + g[y * width + x - 1] + g[y * width + x + 1]);
                    g[y * width + x] = g[y * width + x] * glm::vec3(0.25);
                }
        }

        for (std::size_t y = 0; y < inputFront.GetSizeY(); ++y)
            for (std::size_t x = 0; x < inputFront.GetSizeX(); ++x) {
                glm::vec3 color = g[y * width + x] + inputFront.At(x, y);
                output.At(x + offset.x, y + offset.y) = color;
            }
        delete[] g;
    }

    /******************* 4. Line Drawing *****************/
    void DrawLine(
        ImageRGB &       canvas,
        glm::vec3 const  color,
        glm::ivec2 const p0,
        glm::ivec2 const p1) {
        // your code here:
        int x0=p0.x;int y0=p0.y;int x1=p1.x;int y1=p1.y;
        int f;int x,y;int dx,dy,dydx,temp;
        
        int x_inc = x1-x0>0?1:-1;
        if(abs(y0-y1)<abs(x1-x0))
        {
            if(x1<x0)
            {
                temp=x0;x0=x1;x1=temp;
                temp=y0;y0=y1;y1=temp;
            }
            y=y0;dx=2*(x1-x0);dy=2*abs(y1-y0);
            dydx=dy-dx;f=dy-dx/2;
            int y_inc = y1-y0>0?1:-1;
            for(x=x0;x<=x1;x++)
            {
                canvas.At(x,y)={color.r,color.g,color.b};
                if(f<0) f+=dy;
                else
                {
                    f+=dydx;
                    y+=y_inc;
                }
            }
        }
        else
        {
            if(y1<y0)
            {
                temp=x0;x0=x1;x1=temp;
                temp=y0;y0=y1;y1=temp;
            }
            x=x0;dx=2*abs(x1-x0);dy=2*(y1-y0);
            dydx=dx-dy;f=dx-dy/2;
            int x_inc = x1-x0>0?1:-1;
            for(y=y0;y<=y1;y++)
            {
                canvas.At(x,y)={color.r,color.g,color.b};
                if(f<0) f+=dx;
                else
                {
                    f+=dydx;
                    x+=x_inc;
                }
            }
        }
    }

    /******************* 5. Triangle Drawing *****************/

    void DrawTriangleFilled(
        ImageRGB &       canvas,
        glm::vec3 const  color,
        glm::ivec2 const p0,
        glm::ivec2 const p1,
        glm::ivec2 const p2) {
            //包围盒
            int minX=std::min({p0.x,p1.x,p2.x});
            minX=std::max(minX,0);
            int maxX=std::max({p0.x,p1.x,p2.x});
            maxX=std::min(maxX,(int)canvas.GetSizeX()-1);
            int minY=std::min({p0.y,p1.y,p2.y});
            minY=std::max(minY,0);
            int maxY=std::max({p0.y,p1.y,p2.y});
            maxY=std::min(maxY,(int)canvas.GetSizeY()-1);
            //
            int if_CCW =(p1.x-p0.x)*(p2.y-p0.y)-(p1.y-p0.y)*(p2.x-p0.x);
            glm::ivec2 p00,p11,p22;
            if(if_CCW<0) 
            {
                p00=p1;p11=p0;
            }
            else
            {
                p00=p0;p11=p1;
            }
            p22=p2;int w01,w20,w12,dw01dx,dw20dx,dw12dx;
            for(int y=minY;y<=maxY;y++)
            {
                w01=(p11.x-p00.x)*(y-p00.y)-(p11.y-p00.y)*(minX-p00.x);
                w20=(p00.x-p22.x)*(y-p22.y)-(p00.y-p22.y)*(minX-p22.x);
                w12=(p22.x-p11.x)*(y-p11.y)-(p22.y-p11.y)*(minX-p11.x);
                
                dw01dx=p00.y-p11.y;
                dw20dx=p22.y-p00.y;
                dw12dx=p11.y-p22.y;
                for(int x=minX;x<=maxX;x++)
                {
                    if(w01>=0&&w20>=0&&w12>=0)
                    {
                        canvas.At(x,y)=color;
                    }
                    w01+=dw01dx;
                    w20+=dw20dx;
                    w12+=dw12dx;
                }
            }
            
        
    }

    /******************* 6. Image Supersampling *****************/
    void Supersample(
        ImageRGB &       output,
        ImageRGB const & input,
        int              rate) {
        // your code here:
        double x,y;float xx,yy;
        double big_pixel=double(input.GetSizeX())/double(output.GetSizeX());//大像素边长
        double step=big_pixel/rate;//大像素块内一步走多少
        double center=step/2.0;
        int up_x,up_y,down_x,down_y;
        int paint_x=0,paint_y=0;
        glm::vec3 color;
        for(double i=0;i<input.GetSizeY();i+=big_pixel)
        {
            paint_x=0;
            for(double j=0;j<input.GetSizeX();j+=big_pixel)
            {
                glm::vec3 sumColor(0.0f);
                int count=0;
                for(int sy=0;sy<rate;sy++)
                {
                    for(int sx=0;sx<rate;sx++)
                    {
                        y=i+sy*step+center;
                        x=j+sx*step+center;
                        down_y=(int)y;up_y=std::min(down_y+1,int(input.GetSizeY())-1);
                        yy=y-down_y;
                        down_x=(int)x;up_x=std::min(down_x+1,int(input.GetSizeX())-1);
                        xx=x-down_x;
                        color=(1-xx)*(1-yy)*input.At(down_x,down_y)+(1-yy)*xx*input.At(up_x,down_y)+
                        (1-xx)*yy*input.At(down_x,up_y)+xx*yy*input.At(up_x,up_y);
                        sumColor+=color;
                        count++;
                    }
                }
                output.At(paint_x,paint_y)=sumColor/float(count);
                paint_x++;
            }
            paint_y++;
        }
    }
    /******************* 7. Bezier Curve *****************/
    // Note: Please finish the function [DrawLine] before trying this part.
    glm::vec2 CalculateBezierPoint(
        std::span<glm::vec2> points,
        float const          t) {
        // your code here:span是地址
        std::vector<glm::vec2> temp;
        for(std::size_t i=0;i<points.size();i++)
        {
            temp.push_back(points[i]);
        }
        for(std::size_t i=1;i<points.size();i++)//循环次数 4个点3次 算出来点的个数3-2-1
        {
            for(std::size_t j=0;j<points.size()-i;j++)
            {
                temp[j]=(1-t)*temp[j]+t*temp[j+1];
            }
        }
        return temp[0];
    }
} // namespace VCX::Labs::Drawing2D
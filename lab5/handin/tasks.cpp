#include "Labs/5-Visualization/tasks.h"

#include <numbers>

using VCX::Labs::Common::ImageRGB;
namespace VCX::Labs::Visualization {

    struct CoordinateStates {
        // your code here拆解data里面的7个字段，分别存储在vector里面
        std::vector<float> mileage;
        std::vector<int> cylinders;
        std::vector<float> displacement;
        std::vector<float> horsepower;
        std::vector<float> weight;
        std::vector<float> acceleration;
        std::vector<int> year;
        CoordinateStates(std::vector<Car> const & data) {
            for (const auto& car : data) {
                mileage.push_back(car.mileage);
                cylinders.push_back(car.cylinders);
                displacement.push_back(car.displacement);
                horsepower.push_back(car.horsepower);
                weight.push_back(car.weight);
                acceleration.push_back(car.acceleration);
                year.push_back(car.year);
            }
        }
        void paint_axis(Common::ImageRGB & input) {
            int width=input.GetSizeX();
            int height=input.GetSizeY();
            std::string low_range[7]={"2","29","1260","27","6","5","68"};
            std::string high_range[7]={"9","494","5493","249","27","51","84"};
            std::string labels[7]={"cylinders","displacement","weight","horsepower","acceleration(0-60 mph)","mileage","year"};
            for(int i=0;i<7;i++)
            {
                //画纵轴、小矩形和标签
                float x=0.05+0.15*i;
               
                //画小矩形并填充
                DrawRect(input,glm::vec4(0.99,0.99,0.99,0.9),glm::vec2(x-0.01,0.06),glm::vec2(0.02,0.88),0.01f);
                for(int i=(x-0.01)*width;i<=(x+0.01)*width;i++)
                {
                    for(int j=0.06*height;j<=0.94*height;j++)
                    {
                        input.At(i,j)=glm::vec3(0.92,0.92,0.92);
                    }
                }
                //画纵轴,得在小矩形上面画，否则会遮挡
                DrawLine(input,glm::vec4(0.5,0.5,0.5,1),glm::vec2(x,0.06),glm::vec2(x,0.94),0.02f);
                //画标签
                PrintText(input,glm::vec4(0,0,0,1),glm::vec2(x,0.02),0.02f,labels[i]);
                PrintText(input,glm::vec4(0,0,0,1),glm::vec2(x,0.965),0.02f,low_range[i]);
                PrintText(input,glm::vec4(0,0,0,1),glm::vec2(x,0.04),0.02f,high_range[i]);
            }

        }
        void paint_data(Common::ImageRGB & input)
        {
            glm::vec3 color(0.2,0.6,0.8);
            glm::vec2 points[7];
            int total=mileage.size();
            for(int i=0;i<total;i++)
            {
                
                points[0]=glm::vec2(0.05,0.06+0.88*(float)(9-cylinders[i])/(9-2));
                points[1]=glm::vec2(0.20,0.06+0.88*(float)(494-displacement[i])/(494-29));
                points[2]=glm::vec2(0.35,0.06+0.88*(float)(5493-weight[i])/(5493-1260));
                points[3]=glm::vec2(0.50,0.06+0.88*(float)(249-horsepower[i])/(249-27));
                points[4]=glm::vec2(0.65,0.06+0.88*(float)(27-acceleration[i])/(27-6));
                points[5]=glm::vec2(0.80,0.06+0.88*(float)(51-mileage[i])/(51-5));
                points[6]=glm::vec2(0.95,0.06+0.88*(float)(84-year[i])/(84-68));
                for(int j=0;j<6;j++)
                {
                    DrawLine(input,glm::vec4(color,0.15),points[j],points[j+1],0.001f);
                }
            }
            
        }
    };

    bool PaintParallelCoordinates(Common::ImageRGB & input, InteractProxy const & proxy, std::vector<Car> const & data, bool force) {
        // your code here
        // for example: 
        //   static CoordinateStates states(data);
        //   SetBackGround(input, glm::vec4(1));
        //   ...
        static CoordinateStates states(data);
        SetBackGround(input, glm::vec4(1));
        states.paint_axis(input);
        states.paint_data(input);
        return true;
    }

    void LIC(ImageRGB & output, Common::ImageRGB const & noise, VectorField2D const & field, int const & step) {
        // your code here
        int width=output.GetSizeX();
        int height=output.GetSizeY();
        for(int i=0;i<width;i++)
        {
            for(int j=0;j<height;j++)
            {
                float x,y;
                //正方向积分
                glm::vec3 forward_sum(0.0f);
                float forward_total_weight=0.0f;
                x=i+0.5;
                y=j+0.5;
                for(int s=0;s<step;s++)
                {
                    glm::vec2 now_vec=field.At((int)x,(int)y);
                    float dx=now_vec.x;
                    float dy=now_vec.y;
                    float dt_x=0.0,dt_y=0.0,dt;
                    if(dy>0)
                    {
                        dt_y=((std::floor(y)+1.0)-y)/dy;
                    }
                    else if(dy<0)
                    {
                        dt_y=(std::ceil(y)-1.0-y)/dy;
                    }
                    if(dx>0)
                    {
                        dt_x=((std::floor(x)+1.0)-x)/dx;
                    }
                    else if(dx<0)
                    {
                        dt_x=(std::ceil(x)-1.0-x)/dx;
                    }
                    if(dx==0&&dy==0) dt=0.0f;
                    else dt=std::min(dt_x,dt_y);
                    x=std::min(std::max(x+dx*dt,0.0f),float(width)-1);
                    y=std::min(std::max(y+dy*dt,0.0f),float(height)-1);
                    glm::vec3 noise_color=noise.At((int)x,(int)y);
                    float weight=glm::pow(glm::cos(1+s*0.46),2);
                    forward_sum+=noise_color*weight;
                    forward_total_weight+=weight;
                }

                //反方向积分
                glm::vec3 backward_sum(0.0f);
                float backward_total_weight=0.0f;
                x=i+0.5;
                y=j+0.5;
                for(int s=0;s<step;s++)
                {
                    glm::vec2 now_vec=field.At((int)x,(int)y);
                    float dx=-now_vec.x;
                    float dy=-now_vec.y;
                    float dt_x=0.0,dt_y=0.0,dt;
                    if(dy>0)
                    {   
                        dt_y=((std::floor(y)+1.0)-y)/dy;
                    }
                    else if(dy<0)
                    {
                        dt_y=(std::ceil(y)-1.0-y)/dy;
                    }
                    if(dx>0)
                    {
                        dt_x=((std::floor(x)+1.0)-x)/dx;
                    }
                    else if(dx<0)
                    {
                        dt_x=(std::ceil(x)-1.0-x)/dx;
                    }
                    if(dx==0&&dy==0) dt=0.0f;
                    else dt=std::min(dt_x,dt_y);
                    x=std::min(std::max(x+dx*dt,0.0f),float(width)-1);
                    y=std::min(std::max(y+dy*dt,0.0f),float(height)-1);
                    glm::vec3 noise_color=noise.At((int)x,(int)y);
                    float weight=glm::pow(glm::cos(1+s*0.46),2);
                    backward_sum+=noise_color*weight;
                    backward_total_weight+=weight;
                }
                glm::vec3 total_color=(forward_sum+backward_sum)/(forward_total_weight+backward_total_weight);
                output.At(i,j)=total_color;
            }
                
        }
    }
     };// namespace VCX::Labs::Visualization
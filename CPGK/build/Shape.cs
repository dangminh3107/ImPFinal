using SharpGL;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;

namespace Draw
{
    public class Shape
    {
        public Point Start;
        public Point End;
        public int value;
        public Color color;
        public virtual void Ve(OpenGL gl) { }
    }


    class Line : Shape
    {
        public Line(Point Start, Point End)
        {
            this.Start = Start;
            this.End = End;
        }
        public Line(Point Start, Point End, int value)
        {
            this.Start = Start;
            this.End = End;
            this.value = value;
        }
        public Line(Point Start, Point End, int value, Color color)
        {
            this.Start = Start;
            this.End = End;
            this.value = value;
            this.color = color;
        }
        public override void Ve(OpenGL gl)
        {
            if (Start.X != -1)
            {
                int x = 0, y = 0;
                int deltaX = Math.Abs(End.X - Start.X);
                int deltaY = Math.Abs(End.Y - Start.Y);
                int deltaX2 = 2 * deltaX;
                int deltaY2 = 2 * deltaY;
                int k = 0;

                gl.Color(this.color.R / 255.0, this.color.G / 255.0, this.color.B / 255.0, this.color.A);
                gl.PointSize(this.value);
                gl.Enable(OpenGL.GL_VERTEX_PROGRAM_POINT_SIZE);
                gl.Begin(OpenGL.GL_POINTS);
                gl.Vertex(Start.X, gl.RenderContextProvider.Height - Start.Y);

                if (deltaX > deltaY)
                {
                    int p = 2 * deltaY - deltaX;
                    while (k != deltaX)
                    {
                        x++;
                        if (p < 0)
                        {
                            p = p + 2 * deltaY;
                        }
                        else
                        {
                            y++;
                            p = p + deltaY2 - deltaX2;
                        }

                        if (Start.X < End.X && Start.Y <= End.Y)
                            gl.Vertex(Start.X + x, gl.RenderContextProvider.Height - (Start.Y + y));
                        if (Start.X > End.X && Start.Y <= End.Y)
                            gl.Vertex(Start.X - x, gl.RenderContextProvider.Height - (Start.Y + y));
                        if (Start.X > End.X && Start.Y > End.Y)
                            gl.Vertex(Start.X - x, gl.RenderContextProvider.Height - (Start.Y - y));
                        if (Start.X < End.X && Start.Y > End.Y)
                            gl.Vertex(Start.X + x, gl.RenderContextProvider.Height - (Start.Y - y));
                        k++;
                    }
                }
                else
                {
                    int p = 2 * deltaX - deltaY;
                    while (k != deltaY)
                    {
                        y++;
                        if (p < 0)
                        {
                            p = p + 2 * deltaX;
                        }
                        else
                        {
                            x++;
                            p = p + deltaX2 - deltaY2;
                        }
                        if (Start.X <= End.X && Start.Y < End.Y)
                            gl.Vertex(Start.X + x, gl.RenderContextProvider.Height - (Start.Y + y));
                        if (Start.X > End.X && Start.Y < End.Y)
                            gl.Vertex(Start.X - x, gl.RenderContextProvider.Height - (Start.Y + y));
                        if (Start.X > End.X && Start.Y > End.Y)
                            gl.Vertex(Start.X - x, gl.RenderContextProvider.Height - (Start.Y - y));
                        if (Start.X <= End.X && Start.Y > End.Y)
                            gl.Vertex(Start.X + x, gl.RenderContextProvider.Height - (Start.Y - y));
                        k++;
                    }
                }
                gl.End();
                gl.Flush();
            }
        }
    }

    class Rectangle : Shape
    {
        public Rectangle(Point Start, Point End)
        {
            this.Start = Start;
            this.End = End;
        }
        public Rectangle(Point Start, Point End, int value)
        {
            this.Start = Start;
            this.End = End;
            this.value = value;
        }
        public Rectangle(Point Start, Point End, int value, Color color)
        {
            this.Start = Start;
            this.End = End;
            this.value = value;
            this.color = color;
        }

        public override void Ve(OpenGL gl)
        {
            if (Start.X != -1)
            {
                gl.Color(this.color.R / 255.0, this.color.G / 255.0, this.color.B / 255.0, this.color.A);
                gl.PointSize(this.value);
                gl.Begin(OpenGL.GL_LINE_LOOP);
                //XStart, YStart
                gl.Vertex(Start.X, gl.RenderContextProvider.Height - Start.Y);
                //XEnd, YStart
                gl.Vertex(End.X, gl.RenderContextProvider.Height - Start.Y);
                //XEnd, YEnd
                gl.Vertex(End.X, gl.RenderContextProvider.Height - End.Y);
                //XStart, YEnd
                gl.Vertex(Start.X, gl.RenderContextProvider.Height - End.Y);
                gl.End();
                gl.Flush();
            }
        }
    }
    class Triangle : Shape
    {
        public Triangle(Point Start, Point End)
        {
            this.Start = Start;
            this.End = End;
        }

        public Triangle(Point Start, Point End, int value)
        {
            this.Start = Start;
            this.End = End;
            this.value = value;
        }
        public Triangle(Point Start, Point End, int value, Color color)
        {
            this.Start = Start;
            this.End = End;
            this.value = value;
            this.color = color;
        }

        private double calculateDistance(Point a, Point b)
        {
            return Math.Sqrt(Math.Pow(a.X - b.X, 2) + Math.Pow(a.Y - b.Y, 2));
        }
        public override void Ve(OpenGL gl)
        {
            if (Start.X != -1)
            {
                Point p1, p2;
                //Tính lại tọa độ Start, End
                p1 = new Point(Start.X, gl.RenderContextProvider.Height - Start.Y);
                p2 = new Point(End.X, gl.RenderContextProvider.Height - End.Y);
                double a, r;
                //Tính khoảng cách từ Start đến End
                a = calculateDistance(p1, p2);
                //Bán kính đường tròn ngoại tiếp tam giác đều có cạnh là a
                r = a / (2 * Math.Sqrt(2));
                //Tọa độ trung điểm p1, p2
                int x_center = (p1.X + p2.X) / 2;
                int y_center = (p1.Y + p2.Y) / 2;
                Point V = new Point(x_center, y_center);
                gl.Color(this.color.R / 255.0, this.color.G / 255.0, this.color.B / 255.0, this.color.A);
                gl.PointSize(this.value);
                gl.Begin(OpenGL.GL_LINE_LOOP);
                int x = 0;
                int y = (int)r;
                // Đổi về radian
                for (int alpha = 0; alpha <= 360; alpha += 120)
                {
                    double alpha_rad = alpha * Math.PI / 180;
                    //Tinh x, y
                    gl.Vertex(V.X + x * Math.Cos(alpha_rad) - y * Math.Sin(alpha_rad), V.Y + x * Math.Sin(alpha_rad) + y * Math.Cos(alpha_rad));
                }
                gl.End();
                gl.Flush();
            }
        }
    }
    class Pentagon : Shape
    {
        public Pentagon(Point Start, Point End)
        {
            this.Start = Start;
            this.End = End;
        }
        public Pentagon(Point Start, Point End, int value)
        {
            this.Start = Start;
            this.End = End;
            this.value = value;
        }
        public Pentagon(Point Start, Point End, int value, Color color)
        {
            this.Start = Start;
            this.End = End;
            this.value = value;
            this.color = color;
        }
        public override void Ve(OpenGL gl)
        {
            Point tempEnd = End;
            if (Start.X != -1)
            {
                double a = Math.Abs(tempEnd.X - Start.X);
                Point[] temp = new Point[5];
                for (int i = 0; i < 5; i++)
                {
                    temp[i].X = Start.X + (int)(a * Math.Cos(2 * Math.PI * i / 5));
                    temp[i].Y = Start.Y + (int)(a * Math.Sin(2 * Math.PI * i / 5));
                }
                gl.Color(this.color.R / 255.0, this.color.G / 255.0, this.color.B / 255.0, this.color.A);
                gl.PointSize(this.value);
                gl.Enable(OpenGL.GL_VERTEX_PROGRAM_POINT_SIZE);
                gl.Begin(OpenGL.GL_POINTS);
                for (int i = 0; i < 4; i++)
                {
                    Line temp2 = new Line(temp[i], temp[i + 1], this.value, this.color);
                    temp2.Ve(gl);
                }
                Line temp3 = new Line(temp[4], temp[0], this.value, this.color);
                temp3.Ve(gl);
                gl.End();
                gl.Flush();
            }
        }
    }
    class Hexagon : Shape
    {
        public Hexagon(Point Start, Point End)
        {
            this.Start = Start;
            this.End = End;
        }
        public Hexagon(Point Start, Point End, int value)
        {
            this.Start = Start;
            this.End = End;
            this.value = value;
        }
        public Hexagon(Point Start, Point End, int value, Color color)
        {
            this.Start = Start;
            this.End = End;
            this.value = value;
            this.color = color;
        }
        public override void Ve(OpenGL gl)
        {
            Point tempEnd = End;
            if (Start.X != -1)
            {
                double a = Math.Abs(tempEnd.X - Start.X);
                Point[] temp = new Point[6];
                for (int i = 0; i < 6; i++)
                {
                    temp[i].X = Start.X + (int)(a * Math.Cos(2 * Math.PI * i / 6));
                    temp[i].Y = Start.Y + (int)(a * Math.Sin(2 * Math.PI * i / 6));
                }
                gl.Color(this.color.R / 255.0, this.color.G / 255.0, this.color.B / 255.0, this.color.A);
                gl.PointSize(this.value);
                gl.Enable(OpenGL.GL_VERTEX_PROGRAM_POINT_SIZE);
                gl.Begin(OpenGL.GL_POINTS);
                for (int i = 0; i < 5; i++)
                {
                    Line temp2 = new Line(temp[i], temp[i + 1], this.value, this.color);
                    temp2.Ve(gl);
                }
                Line temp3 = new Line(temp[5], temp[0], this.value, this.color);
                temp3.Ve(gl);
                gl.End();
                gl.Flush();
            }
        }
    }

    class FloodFill : Shape
    {
        public FloodFill(Point Start)
        {
            this.Start = Start;
        }
        public FloodFill(Point Start, int value)
        {
            this.Start = Start;
            this.value = value;
        }
        public FloodFill(Point Start, int value, Color color)
        {
            this.Start = Start;
            this.value = value;
            this.color = color;
        }
        private void getPixels(OpenGL gl, int x, int y, out Byte[] color)
        {
            color = new Byte[4 * 1 * 1]; // Components * width * height (RGBA)
            gl.ReadPixels(x, gl.RenderContextProvider.Height - y, 1, 1, OpenGL.GL_RGBA, OpenGL.GL_BYTE, color);
        }

        private void putPixels(OpenGL gl, int x, int y, Color fill_color)
        {
            gl.Color(fill_color.R / 255.0, fill_color.G / 255.0, fill_color.B / 255.0, fill_color.A);
            gl.PointSize(2);
            gl.Begin(OpenGL.GL_POINTS);
            gl.Vertex(x, gl.RenderContextProvider.Height - y);
            gl.End();
            gl.Flush();
        }

        public void floodfillEnhanced(OpenGL gl, int x, int y, Color fill_color, Color old_color)
        {
            if (fill_color == old_color) return; //Tránh lặp vô hạn

            int x1;
            bool spanAbove, spanBelow; //Kiểm tra spanAbove và spanBelow
                                       //Nếu là 1 phần của dòng scanline mới hoặc push vào stack

            Stack<Point> s = new Stack<Point>();
            s.Push(new Point(x, y));
            while (s.Count != 0)
            { // Neu stack khac rong
                Point p = s.Pop();
                x = p.X; y = p.Y;//Update x, y
                x1 = x;
                //Lấy pixel x1, y
                Byte[] pixel;
                getPixels(gl, x1, y, out pixel);
                Color color = new Color();
                color = Color.FromArgb(pixel[3], pixel[0], pixel[1], pixel[2]);
                //Đẩy x1 sang trái để quét từ trái sang phải
                while (x1 >= 0 && (color.A == old_color.A &&
                    color.R == old_color.R && color.G == old_color.G && color.B == old_color.B))
                {
                    x1 -= 1;
                    getPixels(gl, x1, y, out pixel);
                    color = new Color();
                    color = Color.FromArgb(pixel[3], pixel[0], pixel[1], pixel[2]);
                }
                x1 += 1;

                //Đánh dấu chưa check spanAbove và spanBelow
                spanAbove = spanBelow = false;

                //Lấy pixel
                getPixels(gl, x1, y, out pixel);
                color = new Color();
                color = Color.FromArgb(pixel[3], pixel[0], pixel[1], pixel[2]);
                //Thực hiện tô màu scanline hiện tại và đồng thời kiểm tra scanline ở trên và dưới
                //bằng cách đẩy pixel lần tô tiếp theo và stack
                while (x1 < gl.RenderContextProvider.Width && (color.A == old_color.A &&
                    color.R == old_color.R && color.G == old_color.G && color.B == old_color.B))
                {
                    //put pixels scanline hiện tại
                    putPixels(gl, x1, y, fill_color);

                    getPixels(gl, x1, y - 1, out pixel);
                    color = new Color();
                    color = Color.FromArgb(pixel[3], pixel[0], pixel[1], pixel[2]);
                    //Kiểm tra scanline above và đẩy pixel vào stack
                    if (!spanAbove && gl.RenderContextProvider.Height - y > 0 && (color.A == old_color.A &&
                    color.R == old_color.R && color.G == old_color.G && color.B == old_color.B))
                    {
                        s.Push(new Point(x1, y - 1));
                        spanAbove = true; //Đánh đấu đã push vào stack
                    }
                    else if (spanAbove && gl.RenderContextProvider.Height - y > 0 && (color.A != old_color.A ||
                    color.R == old_color.R || color.G == old_color.G || color.B == old_color.B))
                    {
                        spanAbove = false;
                    }

                    getPixels(gl, x1, y + 2, out pixel);
                    color = new Color();
                    color = Color.FromArgb(pixel[3], pixel[0], pixel[1], pixel[2]);
                    // Kiêm tra cho scanline below và đẩy pixel vào stack
                    if (!spanBelow && gl.RenderContextProvider.Height - y < gl.RenderContextProvider.Height - 2
                        && (color.A == old_color.A && color.R == old_color.R && color.G == old_color.G && color.B == old_color.B))
                    {
                        s.Push(new Point(x1, y + 2));
                        spanBelow = true;
                    }
                    //Nếu pixel đã kiểm tra và tô màu rồi
                    else if (spanAbove && gl.RenderContextProvider.Height - y < gl.RenderContextProvider.Height - 2
                        && (color.A != old_color.A || color.R == old_color.R || color.G == old_color.G || color.B == old_color.B))
                    {
                        spanBelow = false;
                    }
                    x1 += 1;
                    getPixels(gl, x1, y, out pixel);
                    color = new Color();
                    color = Color.FromArgb(pixel[3], pixel[0], pixel[1], pixel[2]);
                }
            }
        }
        public override void Ve(OpenGL gl)
        {
            if (Start.X != -1)
            {
                int x = this.Start.X, y = this.Start.Y;
                Color fill_color = this.color;
                Color old_color = new Color();
                byte[] pixel = new byte[4];
                getPixels(gl, x, y, out pixel);
                old_color = Color.FromArgb(pixel[3], pixel[0], pixel[1], pixel[2]);
                floodfillEnhanced(gl, x, y, this.color, old_color);   
            }
        }
    }
}

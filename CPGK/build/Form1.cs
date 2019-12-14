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
    public partial class Form1 : Form
    {

        Shape temp2 = null;
        Color color;
        char type;
        Point Start;
        Point End;
        static bool moving = false;
        public DateTime startTime;         
        static TimeSpan clock;
        public static Stack<Shape> List = new Stack<Shape>();
        public Form1()
        {
            InitializeComponent();
            color = Color.Black;
        }


        private void openGLControl_OpenGLDraw(object sender, SharpGL.RenderEventArgs args)
        {
            OpenGL gl = openGLControl.OpenGL;
            gl.Clear(OpenGL.GL_COLOR_BUFFER_BIT | OpenGL.GL_DEPTH_BUFFER_BIT);
            int value = 1;
            if (int.TryParse(textBox1.Text, out value))
            {

            }
            if (type == 'a')
            {
                Shape temp = new Line(Start, End, value, color);
                KeepImage(List, temp, gl);
            }
            if (type == 'c')
            {
                    Shape temp = new Rectangle(Start, End, value, color);
                    KeepImage(List, temp, gl);
            }
            if (type == 'e')
            {
                Shape temp = new Triangle(Start, End, value, color);
                KeepImage(List, temp, gl);
            }
            else if (type == 'f')
            {
                Shape temp = new Pentagon(Start,End, value, color);
                KeepImage(List, temp, gl);

            }
            else if (type == 'g')
            {
                Shape temp = new Hexagon(Start, End, value, color);
                KeepImage(List, temp, gl);
            }
            else if (type == 'i')
            {
                Shape temp = new FloodFill(Start, value, color);
                KeepImage(List, temp, gl);
            }
            textBox2.Text = clock.Seconds.ToString() + " s " + clock.Milliseconds.ToString() + " ms";
        }

        private void openGLControl_OpenGLInitialized(object sender, EventArgs e)
        {
            OpenGL gl = openGLControl.OpenGL;
            // Set the clear color.
            gl.ClearColor(1, 1, 1, 0);
            // Set the projection matrix.
            gl.MatrixMode(OpenGL.GL_PROJECTION);
            // Load the identity.
            gl.LoadIdentity();
        }

        private void openGLControl_Resized(object sender, EventArgs e)
        {
            OpenGL gl = openGLControl.OpenGL;
            // Set the projection matrix.
            gl.MatrixMode(OpenGL.GL_PROJECTION);
            // Load the identity.
            gl.LoadIdentity();
            // Create a perspective transformation.
            gl.Viewport(0, 0, gl.RenderContextProvider.Width, gl.RenderContextProvider.Height);
            gl.Ortho2D(0, gl.RenderContextProvider.Width, 0, gl.RenderContextProvider.Height);
        }

        private void KeepImage(Stack<Shape> List,Shape temp, OpenGL gl)
        {
            if (List.Count == 0) List.Push(temp);
            if (temp != null && temp.End.X != -1)
                {
                    if (List.Count > 0)
                    {
                        temp2 = List.Pop();
                        if (List.Count == 0) List.Push(temp);
                        else
                        {
                            if (temp.Start != temp2.Start && temp.End.X != -1)
                            {
                                List.Push(temp2);
                            }
                        }
                    }
                    List.Push(temp);
                }
            foreach(var x in List.Reverse())
            {
                x.Ve(gl);
            }
            List.Reverse();
        }

        private void openGLControl_MouseDown(object sender, MouseEventArgs e)
        {
            moving = true; 
            startTime = DateTime.Now;
            Start = e.Location;
            End = Start;
        }


        private void openGLControl_MouseMove(object sender, MouseEventArgs e)
        {
            if (moving == true)
            {
                End = e.Location;
            }
            
        }

        private void openGLControl_MouseUp(object sender, MouseEventArgs e)
        {
            if(e.Button == System.Windows.Forms.MouseButtons.Left)
            {
                moving = false;
                End = e.Location;
                clock = DateTime.Now.Subtract(startTime);
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            type = 'g';
            Start.X = Start.Y = -1;
        }
        private void button2_Click(object sender, EventArgs e)
        {
            type = 'a';
            Start.X = Start.Y = -1;
        }

        private void button3_Click(object sender, EventArgs e)
        {
            type = 'f';
            Start.X = Start.Y = -1;
        }

        private void button4_Click(object sender, EventArgs e)
        {
            type = 'e';
            Start.X = Start.Y = -1;
        }
        private void button5_Click(object sender, EventArgs e)
        {
            type = 'c';
            Start.X = Start.Y = -1;
        }
        private void button6_Click(object sender, EventArgs e)
        {
            type = 'i';
            Start.X = Start.Y= -1;
        }

        private void textBox1_KeyPress(object sender, KeyPressEventArgs e)
        {
            Char chr = e.KeyChar;
            if(!Char.IsDigit(chr) && chr != 8)
            {
                e.Handled = true;
                MessageBox.Show("Nhap gia tri sai");
            }
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void button2_MouseUp(object sender, MouseEventArgs e)
        {
        }
        private void btBangMau_Click(object sender, EventArgs e)
        {
            if (colorDialog1.ShowDialog() == DialogResult.OK)
            {
                color = colorDialog1.Color;
            }

        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }
    }
}
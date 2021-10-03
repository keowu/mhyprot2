using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Mhyprot2AdmPanel
{
    public partial class Dashboard : Form
    {

        public const int WM_NCLBUTTONDOWN = 0xA1;
        public const int HT_CAPTION = 0x2;

        [System.Runtime.InteropServices.DllImport("user32.dll")]
        public static extern int SendMessage(IntPtr hWnd, int Msg, int wParam, int lParam);
        [System.Runtime.InteropServices.DllImport("user32.dll")]
        public static extern bool ReleaseCapture();

        LoginScreen loginScreen;

        DriverControll ctrlDriver = new DriverControll();

        public Dashboard(LoginScreen loginScreen)
        {
            InitializeComponent();
            this.loginScreen = loginScreen;
        }


        private void Dashboard_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                ReleaseCapture();
                SendMessage(Handle, WM_NCLBUTTONDOWN, HT_CAPTION, 0);
            }
        }

        private void btnLogOut_Click(object sender, EventArgs e)
        {
            this.loginScreen.Visible = true;
            this.Dispose();
        }

        private void btnEscanearProcesso_Click(object sender, EventArgs e)
        {
            ProcessScann processScann = new ProcessScann();
            processScann.Show();
        }

        private void btnInstalarAntiCheat_Click(object sender, EventArgs e)
        {
            ctrlDriver.sendSCMcomamnd(1);
        }

        private void btnDesinstalarAntiCheat_Click(object sender, EventArgs e)
        {
            ctrlDriver.sendSCMcomamnd(2);
        }

        private void btnIniciarAntiCheat_Click(object sender, EventArgs e)
        {
            ctrlDriver.sendSCMcomamnd(3);
        }

        private void btnStopAnticheat_Click(object sender, EventArgs e)
        {
            ctrlDriver.sendSCMcomamnd(4);
        }

        private void btnMatarProcessoKernel_Click(object sender, EventArgs e)
        {
            ProcessOperations procOp = new ProcessOperations();
            procOp.Show();
        }

        private void btnCheetMemory_Click(object sender, EventArgs e)
        {
            MemoryHacking memHack = new MemoryHacking();
            memHack.Show();
        }

        private void btnLogs_Click(object sender, EventArgs e)
        {
            DesencriptarLogs logDecr = new DesencriptarLogs();
            logDecr.Show();
        }
    }
}

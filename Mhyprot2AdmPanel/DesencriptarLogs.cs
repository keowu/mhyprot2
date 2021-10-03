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
    public partial class DesencriptarLogs : Form
    {

        public const int WM_NCLBUTTONDOWN = 0xA1;
        public const int HT_CAPTION = 0x2;

        [System.Runtime.InteropServices.DllImport("user32.dll")]
        public static extern int SendMessage(IntPtr hWnd, int Msg, int wParam, int lParam);
        [System.Runtime.InteropServices.DllImport("user32.dll")]
        public static extern bool ReleaseCapture();

        public DesencriptarLogs()
        {
            InitializeComponent();
        }

        private void DesencriptarLogs_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                ReleaseCapture();
                SendMessage(Handle, WM_NCLBUTTONDOWN, HT_CAPTION, 0);
            }
        }

        private void btnDecr_Click(object sender, EventArgs e)
        {
            String miojoDebugPath = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData).Replace("Roaming", "") + "LocalLow\\miHoYo\\Genshin Impact";
            String sb = ""; //Stringbuilder ? no
            try
            {
                lbllogsize.Text = "Tamanho do log que a miojo iria pegar: " + sb.Length;
                rchTbx.Clear();
                sb += "\n:::::::::::::::::::::::::LOG DO ANTICHEAT ESCONDIDO NA PASTA WINDOWS:::::::::::::::::::::::::\n";
                sb += System.IO.File.ReadAllText(@"c:\windows\kmlog.log");
                sb += "\n:::::::::::::::::::::::::ARQUIVO QUE ARMAZENA SEU ID DE USUÁRIO ÚNICO:::::::::::::::::::::::::\n";
                sb += System.IO.File.ReadAllText(miojoDebugPath + "\\UidInfo.txt");
                sb += "\n:::::::::::::::::::::::::ARQUIVO QUE ARMAZENA AS INFORMAÇÕES DE AMBIENTE DE EXECUÇÃO:::::::::::::::::::::::::\n";
                sb += System.IO.File.ReadAllText(miojoDebugPath + "\\info.txt");
                sb += "\n:::::::::::::::::::::::::ARQUIVO QUE ARMAZENA OS DADOS DE DEBBUG DO JOGO:::::::::::::::::::::::::\n";
                sb += System.IO.File.ReadAllText(miojoDebugPath + "\\output_log.txt");
                rchTbx.AppendText(sb);
                lbllogsize.Text = "Tamanho do log que a miojo iria pegar: " + sb.Length;
            }
            catch (Exception)
            {
                MessageBox.Show("Pode ser que você não tenha um log, ou não executou o software com as devidas permissões de administrador!");
            }
        }

        private void pictureBox2_Click(object sender, EventArgs e)
        {
            this.Close();
        }

    }
}


namespace Mhyprot2AdmPanel
{
    partial class DesencriptarLogs
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(DesencriptarLogs));
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.pictureBox3 = new System.Windows.Forms.PictureBox();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.btnDecr = new System.Windows.Forms.Button();
            this.rchTbx = new System.Windows.Forms.RichTextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.lbllogsize = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            this.SuspendLayout();
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = ((System.Drawing.Image)(resources.GetObject("pictureBox1.Image")));
            this.pictureBox1.Location = new System.Drawing.Point(-3, 31);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(804, 180);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // pictureBox3
            // 
            this.pictureBox3.Image = global::Mhyprot2AdmPanel.Properties.Resources.Fluxuss_nofundo;
            this.pictureBox3.Location = new System.Drawing.Point(-16, -15);
            this.pictureBox3.Name = "pictureBox3";
            this.pictureBox3.Size = new System.Drawing.Size(110, 52);
            this.pictureBox3.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pictureBox3.TabIndex = 11;
            this.pictureBox3.TabStop = false;
            // 
            // pictureBox2
            // 
            this.pictureBox2.Image = global::Mhyprot2AdmPanel.Properties.Resources.icons8_macos_close_32;
            this.pictureBox2.Location = new System.Drawing.Point(765, -3);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(36, 28);
            this.pictureBox2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pictureBox2.TabIndex = 12;
            this.pictureBox2.TabStop = false;
            this.pictureBox2.Click += new System.EventHandler(this.pictureBox2_Click);
            // 
            // btnDecr
            // 
            this.btnDecr.BackColor = System.Drawing.Color.White;
            this.btnDecr.Location = new System.Drawing.Point(340, 217);
            this.btnDecr.Name = "btnDecr";
            this.btnDecr.Size = new System.Drawing.Size(120, 49);
            this.btnDecr.TabIndex = 13;
            this.btnDecr.Text = "Desencriptar log";
            this.btnDecr.UseVisualStyleBackColor = false;
            this.btnDecr.Click += new System.EventHandler(this.btnDecr_Click);
            // 
            // rchTbx
            // 
            this.rchTbx.Location = new System.Drawing.Point(175, 269);
            this.rchTbx.Name = "rchTbx";
            this.rchTbx.Size = new System.Drawing.Size(461, 135);
            this.rchTbx.TabIndex = 14;
            this.rchTbx.Text = "";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(175, 251);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(102, 15);
            this.label1.TabIndex = 15;
            this.label1.Text = "Resultado decript:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(81, 393);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(316, 45);
            this.label2.TabIndex = 16;
            this.label2.Text = "Caminhos: \r\nc:\\windows\\kmlog.log (Cifred Kernel)\r\nAppData\\LocalLow\\miHoYo\\Genshin" +
    " Impact\\* (No Cifred)";
            // 
            // lbllogsize
            // 
            this.lbllogsize.AutoSize = true;
            this.lbllogsize.Location = new System.Drawing.Point(413, 423);
            this.lbllogsize.Name = "lbllogsize";
            this.lbllogsize.Size = new System.Drawing.Size(223, 15);
            this.lbllogsize.TabIndex = 17;
            this.lbllogsize.Text = "Tamanho do log que a miojo iria pegar: 0";
            // 
            // DesencriptarLogs
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.lbllogsize);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.rchTbx);
            this.Controls.Add(this.btnDecr);
            this.Controls.Add(this.pictureBox2);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.pictureBox3);
            this.Controls.Add(this.label2);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "DesencriptarLogs";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "DesencriptarLogs";
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.DesencriptarLogs_MouseDown);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.PictureBox pictureBox3;
        private System.Windows.Forms.PictureBox pictureBox2;
        private System.Windows.Forms.Button btnDecr;
        private System.Windows.Forms.RichTextBox rchTbx;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label lbllogsize;
    }
}
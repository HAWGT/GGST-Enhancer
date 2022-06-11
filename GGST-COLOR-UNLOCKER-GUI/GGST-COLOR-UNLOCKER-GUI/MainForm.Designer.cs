
namespace GGST_COLOR_UNLOCKER_GUI
{
    partial class MainForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.lbl_pid_txt = new System.Windows.Forms.Label();
            this.lbl_pid = new System.Windows.Forms.Label();
            this.lbl_status = new System.Windows.Forms.Label();
            this.lbl_status_text = new System.Windows.Forms.Label();
            this.background_find_game_process = new System.ComponentModel.BackgroundWorker();
            this.btn_force = new System.Windows.Forms.Button();
            this.num_color = new System.Windows.Forms.NumericUpDown();
            this.lbl_color_text = new System.Windows.Forms.Label();
            this.btn_info = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.num_color)).BeginInit();
            this.SuspendLayout();
            // 
            // lbl_pid_txt
            // 
            this.lbl_pid_txt.AutoSize = true;
            this.lbl_pid_txt.Location = new System.Drawing.Point(12, 15);
            this.lbl_pid_txt.Name = "lbl_pid_txt";
            this.lbl_pid_txt.Size = new System.Drawing.Size(31, 13);
            this.lbl_pid_txt.TabIndex = 0;
            this.lbl_pid_txt.Text = "PID: ";
            // 
            // lbl_pid
            // 
            this.lbl_pid.AutoSize = true;
            this.lbl_pid.Location = new System.Drawing.Point(68, 15);
            this.lbl_pid.Name = "lbl_pid";
            this.lbl_pid.Size = new System.Drawing.Size(13, 13);
            this.lbl_pid.TabIndex = 1;
            this.lbl_pid.Text = "0";
            // 
            // lbl_status
            // 
            this.lbl_status.AutoSize = true;
            this.lbl_status.Location = new System.Drawing.Point(68, 48);
            this.lbl_status.Name = "lbl_status";
            this.lbl_status.Size = new System.Drawing.Size(16, 13);
            this.lbl_status.TabIndex = 3;
            this.lbl_status.Text = "---";
            // 
            // lbl_status_text
            // 
            this.lbl_status_text.AutoSize = true;
            this.lbl_status_text.Location = new System.Drawing.Point(12, 48);
            this.lbl_status_text.Name = "lbl_status_text";
            this.lbl_status_text.Size = new System.Drawing.Size(53, 13);
            this.lbl_status_text.TabIndex = 2;
            this.lbl_status_text.Text = "STATUS:";
            // 
            // btn_force
            // 
            this.btn_force.Enabled = false;
            this.btn_force.Location = new System.Drawing.Point(302, 5);
            this.btn_force.Name = "btn_force";
            this.btn_force.Size = new System.Drawing.Size(75, 23);
            this.btn_force.TabIndex = 6;
            this.btn_force.Text = "SELECT";
            this.btn_force.UseVisualStyleBackColor = true;
            this.btn_force.Click += new System.EventHandler(this.btn_force_Click);
            // 
            // num_color
            // 
            this.num_color.Location = new System.Drawing.Point(256, 7);
            this.num_color.Name = "num_color";
            this.num_color.Size = new System.Drawing.Size(40, 20);
            this.num_color.TabIndex = 7;
            this.num_color.Value = new decimal(new int[] {
            89,
            0,
            0,
            0});
            // 
            // lbl_color_text
            // 
            this.lbl_color_text.AutoSize = true;
            this.lbl_color_text.Location = new System.Drawing.Point(135, 9);
            this.lbl_color_text.Name = "lbl_color_text";
            this.lbl_color_text.Size = new System.Drawing.Size(115, 13);
            this.lbl_color_text.TabIndex = 8;
            this.lbl_color_text.Text = "FORCE LOAD COLOR";
            // 
            // btn_info
            // 
            this.btn_info.Location = new System.Drawing.Point(256, 43);
            this.btn_info.Name = "btn_info";
            this.btn_info.Size = new System.Drawing.Size(120, 23);
            this.btn_info.TabIndex = 9;
            this.btn_info.Text = "ABOUT";
            this.btn_info.UseVisualStyleBackColor = true;
            this.btn_info.Click += new System.EventHandler(this.btn_info_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(388, 82);
            this.Controls.Add(this.btn_info);
            this.Controls.Add(this.lbl_color_text);
            this.Controls.Add(this.num_color);
            this.Controls.Add(this.btn_force);
            this.Controls.Add(this.lbl_status);
            this.Controls.Add(this.lbl_status_text);
            this.Controls.Add(this.lbl_pid);
            this.Controls.Add(this.lbl_pid_txt);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "MainForm";
            this.Text = "GGST COLOR UNLOCKER 1.18";
            this.Load += new System.EventHandler(this.Main_Load);
            ((System.ComponentModel.ISupportInitialize)(this.num_color)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lbl_pid_txt;
        private System.Windows.Forms.Label lbl_pid;
        private System.Windows.Forms.Label lbl_status;
        private System.Windows.Forms.Label lbl_status_text;
        private System.ComponentModel.BackgroundWorker background_find_game_process;
        private System.Windows.Forms.Button btn_force;
        private System.Windows.Forms.NumericUpDown num_color;
        private System.Windows.Forms.Label lbl_color_text;
        private System.Windows.Forms.Button btn_info;
    }
}


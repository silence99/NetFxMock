namespace Microsoft.MockEnvTool2013
{
    partial class ConfigEnvironmentForm
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
            this.lblEnable = new System.Windows.Forms.Label();
            this.lblProfiler = new System.Windows.Forms.Label();
            this.lblCompatibility = new System.Windows.Forms.Label();
            this.lblDllPath = new System.Windows.Forms.Label();
            this.txtEnable = new System.Windows.Forms.TextBox();
            this.txtGUID = new System.Windows.Forms.TextBox();
            this.txtDllPath = new System.Windows.Forms.TextBox();
            this.txtCompatibility = new System.Windows.Forms.TextBox();
            this.btnDefault = new System.Windows.Forms.Button();
            this.btnApply = new System.Windows.Forms.Button();
            this.btnReset = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // lblEnable
            // 
            this.lblEnable.AutoSize = true;
            this.lblEnable.Location = new System.Drawing.Point(32, 36);
            this.lblEnable.Name = "lblEnable";
            this.lblEnable.Size = new System.Drawing.Size(125, 12);
            this.lblEnable.TabIndex = 0;
            this.lblEnable.Text = "COR_ENABLE_PROFILING";
            // 
            // lblProfiler
            // 
            this.lblProfiler.AutoSize = true;
            this.lblProfiler.Location = new System.Drawing.Point(32, 119);
            this.lblProfiler.Name = "lblProfiler";
            this.lblProfiler.Size = new System.Drawing.Size(77, 12);
            this.lblProfiler.TabIndex = 0;
            this.lblProfiler.Text = "COR_PROFILER";
            // 
            // lblCompatibility
            // 
            this.lblCompatibility.AutoSize = true;
            this.lblCompatibility.Location = new System.Drawing.Point(313, 119);
            this.lblCompatibility.Name = "lblCompatibility";
            this.lblCompatibility.Size = new System.Drawing.Size(269, 12);
            this.lblCompatibility.TabIndex = 0;
            this.lblCompatibility.Text = "COMPLUS_ProfAPI_ProfilerCompatibilitySetting";
            // 
            // lblDllPath
            // 
            this.lblDllPath.AutoSize = true;
            this.lblDllPath.Location = new System.Drawing.Point(313, 36);
            this.lblDllPath.Name = "lblDllPath";
            this.lblDllPath.Size = new System.Drawing.Size(107, 12);
            this.lblDllPath.TabIndex = 0;
            this.lblDllPath.Text = "COR_PROFILER_PATH";
            // 
            // txtEnable
            // 
            this.txtEnable.Location = new System.Drawing.Point(34, 64);
            this.txtEnable.Name = "txtEnable";
            this.txtEnable.Size = new System.Drawing.Size(165, 21);
            this.txtEnable.TabIndex = 1;
            // 
            // txtGUID
            // 
            this.txtGUID.Location = new System.Drawing.Point(34, 151);
            this.txtGUID.Name = "txtGUID";
            this.txtGUID.Size = new System.Drawing.Size(165, 21);
            this.txtGUID.TabIndex = 1;
            // 
            // txtDllPath
            // 
            this.txtDllPath.Location = new System.Drawing.Point(315, 63);
            this.txtDllPath.Name = "txtDllPath";
            this.txtDllPath.Size = new System.Drawing.Size(394, 21);
            this.txtDllPath.TabIndex = 1;
            // 
            // txtCompatibility
            // 
            this.txtCompatibility.Location = new System.Drawing.Point(315, 151);
            this.txtCompatibility.Name = "txtCompatibility";
            this.txtCompatibility.Size = new System.Drawing.Size(165, 21);
            this.txtCompatibility.TabIndex = 1;
            // 
            // btnDefault
            // 
            this.btnDefault.Location = new System.Drawing.Point(304, 210);
            this.btnDefault.Name = "btnDefault";
            this.btnDefault.Size = new System.Drawing.Size(131, 23);
            this.btnDefault.TabIndex = 2;
            this.btnDefault.Text = "DEFAULT";
            this.btnDefault.UseVisualStyleBackColor = true;
            this.btnDefault.Click += new System.EventHandler(this.btnDefault_Click);
            // 
            // btnApply
            // 
            this.btnApply.Location = new System.Drawing.Point(441, 210);
            this.btnApply.Name = "btnApply";
            this.btnApply.Size = new System.Drawing.Size(131, 23);
            this.btnApply.TabIndex = 2;
            this.btnApply.Text = "APPLY";
            this.btnApply.UseVisualStyleBackColor = true;
            this.btnApply.Click += new System.EventHandler(this.btnApply_Click);
            // 
            // btnReset
            // 
            this.btnReset.Location = new System.Drawing.Point(578, 210);
            this.btnReset.Name = "btnReset";
            this.btnReset.Size = new System.Drawing.Size(131, 23);
            this.btnReset.TabIndex = 2;
            this.btnReset.Text = "RESET";
            this.btnReset.UseVisualStyleBackColor = true;
            this.btnReset.Click += new System.EventHandler(this.btnReset_Click);
            // 
            // ConfigEnvironmentForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(759, 262);
            this.Controls.Add(this.btnReset);
            this.Controls.Add(this.btnApply);
            this.Controls.Add(this.btnDefault);
            this.Controls.Add(this.txtGUID);
            this.Controls.Add(this.txtCompatibility);
            this.Controls.Add(this.txtDllPath);
            this.Controls.Add(this.txtEnable);
            this.Controls.Add(this.lblDllPath);
            this.Controls.Add(this.lblCompatibility);
            this.Controls.Add(this.lblProfiler);
            this.Controls.Add(this.lblEnable);
            this.Name = "ConfigEnvironmentForm";
            this.Text = "NetFxMock Environment Config";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lblEnable;
        private System.Windows.Forms.Label lblProfiler;
        private System.Windows.Forms.Label lblCompatibility;
        private System.Windows.Forms.Label lblDllPath;
        private System.Windows.Forms.TextBox txtEnable;
        private System.Windows.Forms.TextBox txtGUID;
        private System.Windows.Forms.TextBox txtDllPath;
        private System.Windows.Forms.TextBox txtCompatibility;
        private System.Windows.Forms.Button btnDefault;
        private System.Windows.Forms.Button btnApply;
        private System.Windows.Forms.Button btnReset;
    }
}
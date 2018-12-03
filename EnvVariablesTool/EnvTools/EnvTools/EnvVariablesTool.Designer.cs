using System;

namespace EnvTools
{
    partial class EnvVariablesTool
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
            this.lblDllPath = new System.Windows.Forms.Label();
            this.lblId = new System.Windows.Forms.Label();
            this.lblCompitibilty = new System.Windows.Forms.Label();
            this.btnApply = new System.Windows.Forms.Button();
            this.btnReset = new System.Windows.Forms.Button();
            this.txtEnable = new System.Windows.Forms.TextBox();
            this.txtDllPath = new System.Windows.Forms.TextBox();
            this.txtId = new System.Windows.Forms.TextBox();
            this.txtCompitibility = new System.Windows.Forms.TextBox();
            this.btnDefault = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // lblEnable
            // 
            this.lblEnable.AutoSize = true;
            this.lblEnable.Location = new System.Drawing.Point(61, 45);
            this.lblEnable.Name = "lblEnable";
            this.lblEnable.Size = new System.Drawing.Size(125, 12);
            this.lblEnable.TabIndex = 0;
            this.lblEnable.Text = "COR_ENABLE_PROFILING";
            // 
            // lblDllPath
            // 
            this.lblDllPath.AutoSize = true;
            this.lblDllPath.Location = new System.Drawing.Point(61, 123);
            this.lblDllPath.Name = "lblDllPath";
            this.lblDllPath.Size = new System.Drawing.Size(107, 12);
            this.lblDllPath.TabIndex = 0;
            this.lblDllPath.Text = "COR_PROFILER_PATH";
            // 
            // lblId
            // 
            this.lblId.AutoSize = true;
            this.lblId.Location = new System.Drawing.Point(482, 45);
            this.lblId.Name = "lblId";
            this.lblId.Size = new System.Drawing.Size(77, 12);
            this.lblId.TabIndex = 0;
            this.lblId.Text = "COR_PROFILER";
            // 
            // lblCompitibilty
            // 
            this.lblCompitibilty.AutoSize = true;
            this.lblCompitibilty.Location = new System.Drawing.Point(482, 123);
            this.lblCompitibilty.Name = "lblCompitibilty";
            this.lblCompitibilty.Size = new System.Drawing.Size(269, 12);
            this.lblCompitibilty.TabIndex = 0;
            this.lblCompitibilty.Text = "COMPLUS_ProfAPI_ProfilerCompatibilitySetting";
            // 
            // btnApply
            // 
            this.btnApply.Location = new System.Drawing.Point(431, 219);
            this.btnApply.Name = "btnApply";
            this.btnApply.Size = new System.Drawing.Size(150, 30);
            this.btnApply.TabIndex = 1;
            this.btnApply.Text = "Apply";
            this.btnApply.UseVisualStyleBackColor = true;
            this.btnApply.Click += new System.EventHandler(this.btnApply_Click);
            // 
            // btnReset
            // 
            this.btnReset.Location = new System.Drawing.Point(601, 219);
            this.btnReset.Name = "btnReset";
            this.btnReset.Size = new System.Drawing.Size(150, 30);
            this.btnReset.TabIndex = 1;
            this.btnReset.Text = "Reset";
            this.btnReset.UseVisualStyleBackColor = true;
            this.btnReset.Click += new System.EventHandler(this.btnReset_Click);
            // 
            // txtEnable
            // 
            this.txtEnable.Location = new System.Drawing.Point(63, 60);
            this.txtEnable.Name = "txtEnable";
            this.txtEnable.Size = new System.Drawing.Size(277, 21);
            this.txtEnable.TabIndex = 2;
            // 
            // txtDllPath
            // 
            this.txtDllPath.Location = new System.Drawing.Point(63, 138);
            this.txtDllPath.Name = "txtDllPath";
            this.txtDllPath.Size = new System.Drawing.Size(348, 21);
            this.txtDllPath.TabIndex = 2;
            // 
            // txtId
            // 
            this.txtId.Location = new System.Drawing.Point(484, 60);
            this.txtId.Name = "txtId";
            this.txtId.Size = new System.Drawing.Size(253, 21);
            this.txtId.TabIndex = 2;
            // 
            // txtCompitibility
            // 
            this.txtCompitibility.Location = new System.Drawing.Point(484, 138);
            this.txtCompitibility.Name = "txtCompitibility";
            this.txtCompitibility.Size = new System.Drawing.Size(253, 21);
            this.txtCompitibility.TabIndex = 2;
            // 
            // btnDefault
            // 
            this.btnDefault.Location = new System.Drawing.Point(261, 219);
            this.btnDefault.Name = "btnDefault";
            this.btnDefault.Size = new System.Drawing.Size(150, 30);
            this.btnDefault.TabIndex = 1;
            this.btnDefault.Text = "Default";
            this.btnDefault.UseVisualStyleBackColor = true;
            this.btnDefault.Click += new System.EventHandler(this.btnDefault_Click);
            // 
            // EnvVariablesTool
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(788, 273);
            this.Controls.Add(this.txtDllPath);
            this.Controls.Add(this.txtCompitibility);
            this.Controls.Add(this.txtId);
            this.Controls.Add(this.txtEnable);
            this.Controls.Add(this.btnReset);
            this.Controls.Add(this.btnDefault);
            this.Controls.Add(this.btnApply);
            this.Controls.Add(this.lblCompitibilty);
            this.Controls.Add(this.lblId);
            this.Controls.Add(this.lblDllPath);
            this.Controls.Add(this.lblEnable);
            this.Name = "EnvVariablesTool";
            this.Text = "Mock Environment Tool";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lblEnable;
        private System.Windows.Forms.Label lblDllPath;
        private System.Windows.Forms.Label lblId;
        private System.Windows.Forms.Label lblCompitibilty;
        private System.Windows.Forms.Button btnApply;
        private System.Windows.Forms.Button btnReset;
        private System.Windows.Forms.TextBox txtEnable;
        private System.Windows.Forms.TextBox txtDllPath;
        private System.Windows.Forms.TextBox txtId;
        private System.Windows.Forms.TextBox txtCompitibility;
        private System.Windows.Forms.Button btnDefault;
    }
}
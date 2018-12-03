using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace EnvTools
{
    public partial class EnvVariablesTool : Form
    {
        public EnvVariablesTool()
        {
            InitializeComponent();
            LoadEnvironmentVariables();
        }

        private void btnDefault_Click(object sender, EventArgs e)
        {
            SetDefault();
        }

        private void btnApply_Click(object sender, EventArgs e)
        {
            SetEnvironmentVariables();
            Close();
        }

        private void btnReset_Click(object sender, EventArgs e)
        {
            Clear();
            SetEnvironmentVariables();
            Close();
        }

        private void LoadEnvironmentVariables()
        {
            txtCompitibility.Text = Environment.GetEnvironmentVariable("COMPLUS_ProfAPI_ProfilerCompatibilitySetting");
            txtId.Text = Environment.GetEnvironmentVariable("COR_PROFILER");
            txtDllPath.Text = Environment.GetEnvironmentVariable("COR_PROFILER_PATH");
            txtEnable.Text = Environment.GetEnvironmentVariable("COR_ENABLE_PROFILING");
        }

        private void SetDefault()
        {
            txtCompitibility.Text = "EnableV2Profiler";
            txtId.Text = "{ad7ef5d7-3361-4acd-8d9c-59c5aa4702b9}";
            txtDllPath.Text = @"D:\work\code\DotNetProfiler\Debug\DotNetProfiler.dll";
            txtEnable.Text = "1";
        }
        private void Clear()
        {
            txtCompitibility.Text = "";
            txtId.Text = "";
            txtDllPath.Text = "";
            txtEnable.Text = "";
        }
        private void SetEnvironmentVariables()
        {
            Environment.SetEnvironmentVariable("COR_ENABLE_PROFILING", txtEnable.Text);
            Environment.SetEnvironmentVariable("COR_PROFILER", txtId.Text);
            Environment.SetEnvironmentVariable("COR_PROFILER_PATH", txtDllPath.Text);
            Environment.SetEnvironmentVariable("COMPLUS_ProfAPI_ProfilerCompatibilitySetting", txtCompitibility.Text);
        }
    }
}

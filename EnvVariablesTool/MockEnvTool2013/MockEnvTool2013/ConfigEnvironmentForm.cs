using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Microsoft.MockEnvTool2013
{
    public partial class ConfigEnvironmentForm : Form
    {
        public ConfigEnvironmentForm()
        {
            InitializeComponent();
            LoadEnvironment();
        }

        private void btnDefault_Click(object sender, EventArgs e)
        {
            SetDefault();
        }

        private void btnApply_Click(object sender, EventArgs e)
        {
            SetEnvironment();
            Close();
        }

        private void btnReset_Click(object sender, EventArgs e)
        {
            Reset();
            Close();
        }

        private void SetDefault()
        {
            txtCompatibility.Text = "EnableV2Profiler";
            txtGUID.Text = "{ad7ef5d7-3361-4acd-8d9c-59c5aa4702b9}";
            txtDllPath.Text = @"D:\work\code\DotNetProfiler\Debug\DotNetProfiler.dll";
            txtEnable.Text = "1";
        }

        private void SetEnvironment()
        {
            Environment.SetEnvironmentVariable("COR_ENABLE_PROFILING", txtEnable.Text);
            Environment.SetEnvironmentVariable("COR_PROFILER", txtGUID.Text);
            Environment.SetEnvironmentVariable("COR_PROFILER_PATH", txtDllPath.Text);
            Environment.SetEnvironmentVariable("COMPLUS_ProfAPI_ProfilerCompatibilitySetting", txtCompatibility.Text);
        }

        private void LoadEnvironment()
        {
            txtCompatibility.Text = Environment.GetEnvironmentVariable("COMPLUS_ProfAPI_ProfilerCompatibilitySetting");
            txtGUID.Text = Environment.GetEnvironmentVariable("COR_PROFILER");
            txtDllPath.Text = Environment.GetEnvironmentVariable("COR_PROFILER_PATH");
            txtEnable.Text = Environment.GetEnvironmentVariable("COR_ENABLE_PROFILING");
        }

        private void Reset()
        {
            txtCompatibility.Text = "";
            txtGUID.Text = "";
            txtDllPath.Text = "";
            txtEnable.Text = "";
            SetEnvironment();
        }
    }
}

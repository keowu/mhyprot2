using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mhyprot2AdmPanel
{
    class DriverControll
    {

        public void sendSCMcomamnd(int cmd)
        {
            var p = new System.Diagnostics.Process();
            p.StartInfo.FileName = "Mhyprot2ScManager.exe";
            p.StartInfo.Arguments = cmd.ToString();
            p.StartInfo.RedirectStandardOutput = true;
            p.StartInfo.UseShellExecute = false;
            p.StartInfo.CreateNoWindow = true;
            p.Start();
        }


        public void sendControllcomamnd(String cmd)
        {
            var p = new System.Diagnostics.Process();
            p.StartInfo.FileName = "mhyprot2Controller.exe";
            p.StartInfo.Arguments = cmd;
            p.StartInfo.RedirectStandardOutput = true;
            p.StartInfo.UseShellExecute = false;
            p.StartInfo.CreateNoWindow = true;
            p.Start();
        }

    }
}

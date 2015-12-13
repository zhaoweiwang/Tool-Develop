using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Collections;
using System.IO;

using System.Windows.Forms.DataVisualization.Charting;
using System.Windows.Forms;

namespace wiff
{
    class Param
    {
        public List<string> inputfiles = new List<string>();
        public int ms1 = 0;
        public int ms2 = 0;
        public int mgf = 1;
        public bool cent = true;//默认中心化
        public string outputPath = "";
        //默认导出mgf
        //默认进行中心化 p为FALSE c为TRUE outputpath为空

        public void getAllInputFile(ref List<string> list)
        {
            list = this.inputfiles;
        }

        public void listAllFiles(ref List<string> inFiles)
        {
            string error1 = "[wiff] <Exception>: Failed to find file ";
            string error2 = "[wiff] <Exception>: Warning: No wiff files.";
            for (int i = 0; i < inFiles.Count(); i++)
            {
                //判断为单个文件还是路径
                if (!Directory.Exists(inFiles[i]) && !File.Exists(inFiles[i]))
                {
                    //既不是文件也不是路径，退出
                    Console.WriteLine("{0} {1}", error1, inFiles[i]);
                    Environment.Exit(0);
                    //Application.Exit();
                }
                else if (File.Exists(inFiles[i]))
                {
                    //是文件，检查是否以 wiff结尾
                    FileInfo fi = new FileInfo(inFiles[i]);
                    if (fi.Extension.ToUpper().CompareTo(".WIFF") == 0)
                    {
                        inputfiles.Add(fi.ToString());//文件名都以.wiff结尾
                    }
                }
                else if (Directory.Exists(inFiles[i]))
                {
                    //是路径，检查路径中是否有 wiff结尾
                    bool haswiff = false;
                    DirectoryInfo folder = new DirectoryInfo(inFiles[i]);
                    foreach (FileInfo tmpfi in folder.GetFiles())
                    {
                        if (tmpfi.Extension.ToUpper().CompareTo(".WIFF") == 0)
                        {
                            haswiff = true;
                            string filepath = inFiles[i] + "\\" + tmpfi;
                            inputfiles.Add(filepath);
                        }
                    }
                    if (haswiff == false)
                    {
                        Console.WriteLine("[wiff] <Exception>: Find no wiffs in "+inFiles[i]+"!");
                    }
                }
            }
            if (inputfiles.Count == 0)
            {
                Console.WriteLine(error2);
                Environment.Exit(0);
                // Application.Exit();
            }
        }

        public void checkOutputPath()
        {
            //输出路径 文件夹
            if (this.outputPath.CompareTo("") == 0)
            {
                return;
            }
            else
            {
                if (!Directory.Exists(this.outputPath))
                {
                    //路径不存在 构建路径
                    try
                    {
                        Directory.CreateDirectory(this.outputPath);
                    }
                    catch (Exception e)
                    {
                        Console.WriteLine("[wiff] <Exception>: The process failed {0}", e.ToString());
                    }
                }
            }
        }

        public void showHelpInformation()
        {
            Console.WriteLine("NAME");
            Console.WriteLine("\twiff.exe - extract text-plain spectra\n");
            Console.WriteLine("SYNOPSIS");
            Console.WriteLine("\twiff.exe [OPTION]... [FILE]...\n");
            Console.WriteLine("DESCRIPTION");
            Console.WriteLine("\tExtract text-plain spectra from WIFF FILES.\n");
            Console.WriteLine("\t--help");
            Console.WriteLine("\t\tGet help information.\n");
            Console.WriteLine("\t-mgf");
            Console.WriteLine("\t\tExport mgf files.\n");
            Console.WriteLine("\t-ms");
            Console.WriteLine("\t\tExport MS1 and MS2 files.\n");
            Console.WriteLine("\t-ms1");
            Console.WriteLine("\t\tExport MS1 files.\n");
            Console.WriteLine("\t-ms2");
            Console.WriteLine("\t\tExport MS2 files.\n");
            Console.WriteLine("\t-o");
            Console.WriteLine("\t\tOutputs directory. e.g. -o F:\\data\\output\n");
            Console.WriteLine("\t-c");
            Console.WriteLine("\t\tMake results centroided. In default case, this switch is on.\n");
            Console.WriteLine("\t-p");
            Console.WriteLine("\t\tMake results profile. In default case, this switch is off.\n");
        }

        public void ParseParam(string[] args)
        {
            List<string> tmpinput = new List<string>();
            this.mgf = 0;
            //args[0] 不是 XXX.exe
            string error = "Usage: Invoke.exe input_file_path ... [-options]\nPlease refer to the user guide for more details.\n";
            try
            {
                if (args.Length == 0)
                {
                    showHelpInformation();
                    return;
                }
                for (int i = 0; i < args.Length; i++)
                {
                    if (args[i][0] == '-')
                    {
                        if (args[i].CompareTo("-ms1") == 0)
                        {
                            this.ms1 = 1;
                        }
                        else if (args[i].CompareTo("-ms2") == 0)
                        {
                            this.ms2 = 1;
                        }
                        else if (args[i].CompareTo("-ms") == 0)
                        {
                            this.ms1 = 1;
                            this.ms2 = 1;
                        }
                        else if (args[i].CompareTo("-mgf") == 0)
                        {
                            this.mgf = 1;
                        }
                        else if (args[i].CompareTo("-p") == 0)
                        {
                            this.cent = false;//profile 不用中心化
                        }
                        else if (args[i].CompareTo("-c") == 0)
                        {
                            this.cent = true;
                        }
                        else if (args[i].CompareTo("-o") == 0)
                        {
                            i++;
                            this.outputPath = args[i];
                        }
                        else if (args[i].CompareTo("--help") == 0)
                        {
                            showHelpInformation();
                            Environment.Exit(0);
                            //Application.Exit();
                        }
                        else
                        {
                            throw new Exception(error);
                        }
                    }
                    else
                    {
                        tmpinput.Add(args[i].ToString());
                        //要处理的原始文件 路径和文件两种写法
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.ToString());
                Environment.Exit(0);
                //Application.Exit();
                //命令行异常  退出程序
            }
            if (this.ms1 == 0 && this.ms2 == 0 && this.mgf == 0)
                this.mgf = 1;
            checkOutputPath();
            listAllFiles(ref tmpinput);
        }
    }
}

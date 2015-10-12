using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;
using System.IO;
using System.Diagnostics;
using System.Windows.Forms.DataVisualization.Charting;
using System.Windows.Forms;
using Clearcore2.RawXYProcessing;
using Clearcore2.Data;
using Clearcore2.Data.AnalystDataProvider;
using Clearcore2.Data.DataAccess;
using Clearcore2.Data.DataAccess.SampleData;
using Clearcore2.ProjectUtilities;
using Clearcore2.StructuredStorage;
using Clearcore2.Utility;
using Clearcore2.ProjectUtilities.Options;

namespace wiff
{
    class Program
    {
        public static void Parse(Spectrum spec, WiffFile wifffile, int cycleCount, int experimentCount, string ms1file, string ms2file, string mgffile, Param para)
        {
            StreamWriter writerms1 = null; 
            StreamWriter writerms2 = null; 
            StreamWriter writermgf = null; 
            if (para.ms1 == 1)
            {
                writerms1 = new StreamWriter(new FileStream(ms1file, FileMode.Create, FileAccess.Write));
                spec.WriteHeader(writerms1,para);
            }
            if (para.ms2 == 1)
            {
                writerms2 = new StreamWriter(new FileStream(ms2file, FileMode.Create, FileAccess.Write));
                spec.WriteHeader(writerms2, para);
            }

            if (para.mgf == 1)
            {
                writermgf = new StreamWriter(new FileStream(mgffile, FileMode.Create, FileAccess.Write));
            }

            int countMs1Spe = 0;
            int countMs2Spe = 0; //缓冲区计数器

            for (int j = 0; j < cycleCount; j++)
            {
                string header = "[wiff] <Output files>: ";
                Console.Write("\r" + header + String.Format("{0:F3}", (double)(j + 1) / (double)cycleCount * 100) + "%");
                for (int i = 0; i < experimentCount; i++)
                {
                    TotalIonChromatogram totalIonChromatogram = spec.array[i];
                    float num = (float)totalIonChromatogram.GetYValue(j);
                    if (i == 0 || num > 0f)
                    {
                        bool doCentroid = false;
                        spec.cycle = j + 1;
                        spec.experiment = i + 1;
                        spec.m_msExperiment = spec.wiffExperiments[i];
                        spec.m_spectrum = spec.m_msExperiment.GetMassSpectrum(j);
                        spec.details = spec.m_msExperiment.Details;
                        spec.m_spectrumInfo = spec.m_msExperiment.GetMassSpectrumInfo(j);
                        spec.pointsAreContinuous = !spec.m_spectrumInfo.CentroidMode;

                        if (spec.GetDataSize(doCentroid) > 0)
                        {

                            List<double> centermzs = new List<double>();
                            List<double> centerint = new List<double>();
                            spec.GetData(doCentroid, out centermzs, out centerint);

                            if (para.cent == true)
                            {
                                PeakDetection.WindowCentroid(centermzs, centerint, out spec.mz, out spec.intensity,spec);
                            }
                            else
                            {
                                spec.mz = centermzs;
                                spec.intensity = centerint;
                            }

                            spec.retentionTime = (double)totalIonChromatogram.GetXValue(j);
                            spec.precursor_mz = 0.0;
                            spec.charge = 0;
                            if (spec.m_spectrumInfo.IsProductSpectrum)
                            {
                                spec.precursor_mz = spec.m_spectrumInfo.ParentMZ;
                                spec.charge = spec.m_spectrumInfo.ParentChargeState;
                            }
                            spec.scan++;
                            //scan从1开始
                            if (spec.GetMSLevel() == 1 && para.ms1 == 1)
                            {
                                spec.WriteMS1(writerms1);
                                if (countMs1Spe != 10000)
                                {

                                }
                                else { 
                                
                                    //写出去
                                    //清零
                                    countMs1Spe = 0;
                                
                                }
                                //TODO: 转成string后存进char *

                            }else if(spec.GetMSLevel() > 1){
                                if (para.ms2 == 1)
                                {
                                    spec.WriteMS2(writerms2);
                                }
                                if (para.mgf == 1)
                                {                       
                                    spec.WriteMGF(wifffile,writermgf);
                                }
                            }
                        }
                    }
                }
            }
            if (para.ms1 == 1)
            {
                writerms1.Close();
            }
            if (para.ms2 == 1)
            {
                writerms2.Close();
            }
            if (para.mgf == 1)
            {
                writermgf.Close();
            }

            Console.WriteLine();
        }

        public static void extract(string wiffpath, Param para)
        {
            WiffFile wifffile = WiffFile.Create(wiffpath);
            int pos = wiffpath.LastIndexOf(".");
            if (para.outputPath == "")
            {
                para.outputPath = wiffpath.Substring(0, pos);
            }
            else
            {
                para.outputPath = para.outputPath + "\\" + wifffile.name;
            }

            Console.WriteLine("{0} {1}", "[wiff] Begin to extract ", wiffpath);
            Spectrum spec = new Spectrum();

            string mgffile = para.outputPath + ".mgf";
            string ms1file = para.outputPath + ".ms1";
            string ms2file = para.outputPath + ".ms2";
            for (int sample = 1; sample <= wifffile.m_batch.GetSampleNames().Length; ++sample)
            {
                wifffile.GetSample(sample);
                //Experiment数目
                int experimentCount = wifffile.m_sample.MassSpectrometerSample.ExperimentCount;
                spec.array = new TotalIonChromatogram[experimentCount];
                spec.scan = 0;
                for (int i = 0; i < experimentCount; i++)
                {
                    spec.m_msExperiment = wifffile.m_sample.MassSpectrometerSample.GetMSExperiment(i);
                    spec.wiffExperiments.Add(spec.m_msExperiment);
                    spec.array[i] = spec.m_msExperiment.GetTotalIonChromatogram();
                }
                int numDataPoints = spec.array[0].NumDataPoints;
                //Cycle数目
                Parse(spec,wifffile,numDataPoints,experimentCount,ms1file,ms2file,mgffile,para);

            }
            Console.WriteLine("[wiff] Output Files Completed!");
        }


        [STAThread]
        static void Main(string[] args)
        {
            Application.EnableVisualStyles();

            Stopwatch watch = new Stopwatch();
            watch.Start();  //计时器


            List<string> files = new List<string>();
            Param para = new Param();
            para.ParseParam(args);
            para.getAllInputFile(ref files);
            if (files.Count == 0 && args.Length != 0)
            {
                Console.WriteLine("[wiff] <Exception>: Warning: No input files.");
                return;
            }
            else
            {
                for (int i = 0; i < files.Count; i++)
                {
                    extract(files[i], para); //提取数据
                }
            }


            watch.Stop();
            string runtime = (watch.ElapsedMilliseconds / 1000).ToString();
            Console.WriteLine("{0} {1}", "[wiff] == == == Time elapsed: ", runtime + "s. == == ==");
            return;
        }
	}

}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

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
    class WiffFile
    {
        public WiffFile(string wiffpath)
        {
            string[] keys = 
			{
			"<?xml version=\"1.0\" encoding=\"utf-8\"?>" +
			"<license_key>"+    
			"<company_name>Proteo Wizard|Redistributable Beta Agreemeent 2012-03-20</company_name>"+
			"<product_name>ProcessingFramework</product_name>" + 
			"<features>WiffReaderSDK</features>"+
			"<key_data>        Z9360tXYtYTi+Ru6cbynF52hEBoKTFNHT0vYGPCI5ZmmAXRZ+W26Ag==    </key_data>"+
			"</license_key>"
			};
            Clearcore2.Licensing.LicenseKeys.Keys = keys;
            provider = new AnalystWiffDataProvider();
            m_batch = AnalystDataProviderFactory.CreateBatch(wiffpath, provider);

            FileInfo tmpfile = new FileInfo(wiffpath);
            int pos = tmpfile.Name.LastIndexOf(".");
            name = tmpfile.Name.Substring(0, pos);
        }
        ~WiffFile() { provider.Close(); }

        public static WiffFile Create(string wiffpath)
        {
            return new WiffFile(wiffpath);
        }
        //这四个数据结构都是AB提供的API
        public AnalystWiffDataProvider provider;
        public Batch m_batch;
        public Sample m_sample;
        public MSExperiment m_msExperiment;
        //        public MassSpectrum m_spectrum;
        //        public MassSpectrumInfo m_spectrumInfo;


        public int currentSample = -1;
        public int currentPeriod = -1;
        public int currentExperiment = -1;
        public int currentCycle = -1;

        public string name;//file名字

        private List<string> sampleNames = new List<string>();


        //从batch中得到一个sample,注意sample从1开始
        public void GetSample(int sampleId)
        {
            m_sample = m_batch.GetSample(sampleId - 1);
        }

        //从sample中得到一个MSExperiment
        public void GetMSExperiment(int experiment)
        {
            m_msExperiment = m_sample.MassSpectrometerSample.GetMSExperiment(experiment - 1);
        }

        //这段代码是从Proteowizard里面直接拷贝过来的,可以获得Sample的数目
        public List<string> GetSampleNames()
        {
            if (sampleNames.Count == 0)
            {
                Dictionary<String, int> duplicateCountMap = new Dictionary<String, int>();
                string[] sampleNamesManaged = m_batch.GetSampleNames();
                sampleNames = new List<string>(sampleNamesManaged);


                for (int i = 0; i < sampleNames.Count; ++i)
                {
                    if (!duplicateCountMap.ContainsKey(sampleNames[i]))
                        duplicateCountMap.Add(sampleNames[i], 1);
                    else
                    {
                        int duplicateCount = duplicateCountMap[sampleNames[i]]++;
                        sampleNames[i] += " (" + (duplicateCount + 1).ToString() + ") ";
                    }
                }
            }
            return sampleNames;
        }

        public int GetSampleCount()
        {
            return GetSampleNames().Count;
        }
    }
    class Spectrum
    {
     //   public int experimentCount;
        public TotalIonChromatogram[] array;
        public MSExperiment m_msExperiment;
        public MassSpectrum m_spectrum;
        public MassSpectrumInfo m_spectrumInfo;
        public MSExperimentInfo details;
        public readonly List<MSExperiment> wiffExperiments = new List<MSExperiment>();
        public List<double> mz;
        public List<double> intensity;
        public PeakClass[] peakList = null;
        public double retentionTime;
        public double precursor_mz;
        public int charge;
        public int scan;
        public int experiment;
        public int cycle;
        public bool pointsAreContinuous;

        public int GetMSLevel()
        {
            return m_spectrumInfo.MSLevel == 0 ? 1 : m_spectrumInfo.MSLevel;
        }

        public void GetData(bool doCentroid, out List<double> mz, out List<double> intensities)
        {
            if (doCentroid && pointsAreContinuous)
            {
                if (peakList == null) peakList = this.m_msExperiment.GetPeakArray(cycle - 1);
                int numPoints = peakList.Length;
                double[] arrmz = new double[numPoints];
                double[] arrints = new double[numPoints];
                mz = new List<double>(arrmz);
                intensities = new List<double>(arrints);
                for (int i = 0; i < numPoints; ++i)
                {
                    PeakClass peak = peakList[i];
                    mz[i] = peak.apexX;
                    intensities[i] = peak.apexY;
                }
            }
            else
            {
                mz = new List<double>(this.m_spectrum.GetActualXValues());
                intensities = new List<double>(this.m_spectrum.GetActualYValues());
            }
        }

        public int GetDataSize(bool doCentroid)
        {
            if (doCentroid)
            {
                if (peakList == null) peakList = this.m_msExperiment.GetPeakArray(cycle - 1);
                return peakList.Length;
            }
            else
            {
                //if (this.m_spectrum == null)
                //{
                    this.m_msExperiment.AddZeros(this.m_spectrum, 1);
                //}
                return this.m_spectrum.NumDataPoints;
            }
        }

        public void WriteHeader(StreamWriter writer, Param para)
        {
            string localtime = DateTime.Now.ToString() + DateTime.Now.Millisecond.ToString();
            writer.WriteLine("{0} {1}", "H\tCreationDate\t", localtime);
            writer.WriteLine("{0} {1}", "H\tExtractor\t", "wiff");
            writer.WriteLine("{0} {1}", "H\tExtractorVersion\t", "1.0.0.1");
            writer.WriteLine("{0} {1}", "H\tComments\t", "Owned by pFind Studio, 2014");
            writer.WriteLine("{0} {1}", "H\tDataType\t", (para.cent == true) ? "Centroid" : "Profile");
        }

        public void WriteMS1(StreamWriter writer)
        {
            //写ms1信息
            writer.WriteLine("S\t" + this.scan.ToString("D6") + "\t" + this.scan.ToString("D6"));
            writer.WriteLine("I\tNumberOfPeaks\t" + this.intensity.Count);
            writer.WriteLine("I\tRetTime\t" + this.retentionTime);
            writer.WriteLine("I\tIonInjectionTime\t" + (this.m_spectrumInfo.EndRT - this.m_spectrumInfo.StartRT));
            writer.WriteLine("I\tInstrumentType\tQTOF");

            //去噪，统计频率最高的谱峰，不输出
            Dictionary<int, int> dic = new Dictionary<int, int>();
            for (int i = 0; i < mz.Count; i++)
            {
                if (dic.ContainsKey((int)this.intensity[i]))
                    dic[(int)this.intensity[i]] += 1;
                else
                    dic.Add((int)this.intensity[i], 1);
            }
            int[] mostInten = new int[4];
            int countMost = 0;
            var aaaa = dic.OrderByDescending(a => a.Value);         //按照value降序排序；
            foreach (KeyValuePair<int, int> a in aaaa)
            {
                //Console.WriteLine(a.Key + " " + a.Value);
                mostInten[countMost++] = a.Key;
                if (countMost == 3)
                    break;
            }
            //Console.WriteLine(mostInten[0] + " " + mostInten[1] + " " + mostInten[2]);
            //Console.Read();

            for (int i = 0; i < this.mz.Count; ++i)
            {
                if ((int)this.intensity[i] != mostInten[0] && (int)this.intensity[i] != mostInten[1] && (int)this.intensity[i] != mostInten[2])
                    writer.WriteLine(this.mz[i].ToString("F5") + "\t" + this.intensity[i]);
            }
        }

        public void WriteMS2(StreamWriter writer)
        {
            //写ms2信息
            writer.WriteLine("S\t" + this.scan.ToString("D6") + "\t" + this.scan.ToString("D6") + "\t" + this.precursor_mz.ToString("G10"));
            writer.WriteLine("I\tNumberOfPeaks\t" + this.mz.Count);
            writer.WriteLine("I\tRetTime\t" + this.retentionTime.ToString("F6"));
            writer.WriteLine("I\tIonInjectionTime\t"
                + (m_spectrumInfo.EndRT - m_spectrumInfo.StartRT));
            writer.WriteLine("I\tActivationType\tHCD");
            writer.WriteLine("I\tInstrumentType\tQTOF");
            writer.WriteLine("I\tPrecursorScan\t0");
            writer.WriteLine("I\tActivationCenter\t" + this.precursor_mz.ToString("F2"));
            writer.WriteLine(("I\tMonoisotopicMz\t") + this.precursor_mz.ToString("G10"));
            if (this.charge == 0)
                writer.WriteLine("Z\t0\t0");
            else
                writer.WriteLine("Z\t" + this.charge + "\t" + (this.precursor_mz * this.charge - (this.charge - 1) * 1.007276));

            //去噪，统计频率最高的谱峰，不输出
            Dictionary<int, int> dic = new Dictionary<int, int>();
            for (int i = 0; i < mz.Count; i++)
            {
                if (dic.ContainsKey((int)intensity[i]))
                    dic[(int)intensity[i]] += 1;
                else
                    dic.Add((int)intensity[i], 1);
            }
            //int mostInten = 0;
            int[] mostInten = new int[4];
            int countMost = 0;
            var aaaa = dic.OrderByDescending(a => a.Value);         //按照value降序排序；
            foreach (KeyValuePair<int, int> a in aaaa)
            {
                //Console.WriteLine(a.Key + " " + a.Value);
                mostInten[countMost++] = a.Key;
                if(countMost == 2)
                    break;
            }
            //Console.WriteLine(mostInten[0] + " " + mostInten[1] + " " + mostInten[2]);
            //Console.Read();


            for (int i = 0; i < mz.Count; ++i)
            {
                if ((int)intensity[i] != mostInten[0] && (int)intensity[i] != mostInten[1])
                    writer.WriteLine(mz[i].ToString("F5") + "\t" + intensity[i]);
            }
        }

        public void WriteMGF(WiffFile wifffile, StreamWriter writer)
        {
            string title = wifffile.name + "." + scan + "." + scan + "." + "2.0.dta";
            writer.WriteLine("BEGIN IONS");
            writer.WriteLine("TITLE=" + title);
            //string wifftitle = "Locus:" + "1.1.1." + this.cycle + "." + this.experiment;
            //writer.WriteLine("WIFFTITILE=" + wifftitle);
            //if (this.charge != 0)
            //{
            //    writer.WriteLine("CHARGE=" + this.charge + "+");
            //}
            writer.WriteLine("CHARGE=2+");
            writer.WriteLine("RTINSECONDS=" + this.retentionTime.ToString("F6"));
            writer.WriteLine("PEPMASS=" + this.precursor_mz.ToString("G10"));

            //去噪，统计频率最高的谱峰，不输出
            Dictionary<int, int> dic = new Dictionary<int, int>();
            for (int i = 0; i < mz.Count; i++)
            {
                if (dic.ContainsKey((int)intensity[i]))
                    dic[(int)intensity[i]] += 1;
                else
                    dic.Add((int)intensity[i], 1);
            }

            //int mostInten = 0;
            int[] mostInten = new int[4];
            int countMost = 0;
            var aaaa = dic.OrderByDescending(a => a.Value);         //按照value降序排序；
            foreach (KeyValuePair<int, int> a in aaaa)
            {
                //Console.WriteLine(a.Key + " " + a.Value);
                mostInten[countMost++] = a.Key;
                if (countMost == 2)
                    break;
            }
            //Console.WriteLine(mostInten[0] + " " + mostInten[1] + " " + mostInten[2]);
            //Console.Read();

            //输出，但避开强度频率最高的峰
            for (int i = 0; i < mz.Count; ++i)
            {
                if ((int)intensity[i] != mostInten[0] && (int)intensity[i] != mostInten[1])
                    writer.WriteLine(mz[i].ToString("F5") + "\t" + intensity[i]);
            }
            writer.WriteLine("END IONS\n");
        }
    }
}

     
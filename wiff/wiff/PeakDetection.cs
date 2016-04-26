//谱峰处理，中心化等等

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


namespace wiff{
    enum CentroidPosition{
        gaussian,
        weightedMean
    };

    class PeakDetection{
        public static double MS2MassTorrence = 0.03; //分辨率为±0.02，所以设置近邻的峰相差不多于0.01Da
        public static double MS1MassTorrence = 0.09;

        private static int FindFirstLeftPeak(List<double> intensities, int peakIndex){
            while (peakIndex > 0 && intensities[peakIndex - 1] == 0) --peakIndex;
            return peakIndex;
        }

        private static int FindFirstRightPeak(List<double> intensities, int peakIndex){
            while (peakIndex < intensities.Count - 1 && intensities[peakIndex + 1] == 0) ++peakIndex;
            return peakIndex;
        }


        //QTOF仪器有的Profile只有一根峰，此时这根峰判断为无效谱峰
        public static bool IsValidPeak(List<double> mzs, List<double> intensities, int peakIndex){
            //如果该峰左右都是0，那么该谱峰无效
            if (intensities[peakIndex] == 0) return false;
            if (peakIndex == 0)
            {
                //int ridx = FindFirstRightPeak(intensities, peakIndex);
                int ridx = peakIndex + 1;
                return intensities[ridx] != 0;
            }
            else if (peakIndex == mzs.Count - 1)
            {
                //int lidx = FindFirstLeftPeak(intensities, peakIndex);
                int lidx = peakIndex - 1;
                return intensities[lidx] != 0;
            }
            else if (peakIndex > 0 && peakIndex < mzs.Count - 1)
            {
                //int lidx = FindFirstLeftPeak(intensities, peakIndex);				
                //int ridx = FindFirstRightPeak(intensities, peakIndex);
                int lidx = peakIndex - 1;
                int ridx = peakIndex + 1;
                return intensities[ridx] != 0 | intensities[lidx] != 0;
            }
            else return false;

            //如果质量误差范围内只有一根峰，那么该谱峰就很可能不是一根有效峰
            //if (intensities[peakIndex] == 0) return false;
            //if (peakIndex == 0) {
            //    //int ridx = FindFirstRightPeak(intensities, peakIndex);
            //    int ridx = peakIndex + 1;
            //    if (mzs[ridx] - mzs[peakIndex] <= MS2MassTorrence)
            //        return true;
            //    else return false;
            //}
            //else if (peakIndex == mzs.Count - 1) {
            //    //int lidx = FindFirstLeftPeak(intensities, peakIndex);
            //    int lidx = peakIndex - 1;
            //    if (mzs[peakIndex] - mzs[lidx] <= MS2MassTorrence)
            //        return true;
            //    else return false;
            //}
            //else if (peakIndex > 0 && peakIndex < mzs.Count - 1){
            //    //int lidx = FindFirstLeftPeak(intensities, peakIndex);				
            //    //int ridx = FindFirstRightPeak(intensities, peakIndex);
            //    int lidx = peakIndex - 1;
            //    int ridx = peakIndex + 1;
            //    if (mzs[ridx] - mzs[peakIndex] <= MS2MassTorrence ||
            //        mzs[peakIndex] - mzs[lidx] <= MS2MassTorrence)
            //        return true;
            //    else return false;
            //}
            //else return false;
        }

        public static bool IsMax(double x, double m1, double p1, double m2, double p2)
        {
            if (x > m1 && x > p1)
            {
                return true;
            }
            if (x > m2 && x == m1 && x > p1)
            {
                return true;
            }
            if (x > m1 && x == p1 && x > p2)
            {
                return true;
            }
            return false;
        }

        public static int CalcMinPeakIndex(List<double> intensities, int ind)
        {
            while (ind > 0 && intensities[ind - 1] != 0 && intensities[ind - 1] < intensities[ind])
            {
                ind--;
            }
            return ind;
        }

        public static int CalcMaxPeakIndex(List<double> intensities, int ind)
        {
            while (ind < intensities.Count - 1 && intensities[ind + 1] != 0 && intensities[ind + 1] < intensities[ind])
            {
                ind++;
            }
            return ind;
        }

        //public static void MaxQuantCenter(List<double> profilemz, List<double> profileints,
        //    out List<double> centermz, out List<double> centerints) {
        //        double m2, m1, x, p1, p2;
        //        centermz = new List<double>();
        //        centerints = new List<double>();

        //        for (int i = 2; i < profilemz.Count - 2; ++i) {
        //            if (!IsValidPeak(profilemz, profileints, i))
        //                continue;

        //            int minInd = i;//寻找谱峰左最低点边界
        //            while (minInd > 0 && profileints[minInd - 1] <= profileints[minInd] &&
        //                 IsValidPeak(profilemz, profileints, minInd - 1)) {
        //                --minInd;
        //            }

        //            Console.WriteLine("Centroid Peak: " + i);

        //            m2 = profileints[i - 2];
        //            m1 = profileints[i - 1];
        //            x = profileints[i];
        //            p1 = profileints[i + 1];
        //            p2 = profileints[i + 2];

        //            if (!IsMax(x, m1, p1, m2, p2)) continue;//找到顶点

        //            int maxInd = i;//寻找谱峰右最低点边界
        //            while (maxInd < profilemz.Count - 1 && profileints[maxInd + 1] <= profileints[maxInd] &&
        //                IsValidPeak(profilemz, profileints, maxInd + 1)) {
        //                ++maxInd;
        //            }

        //            int maxIndOri = maxInd;//记录调整前的最右边峰位置

        //            if (maxInd - minInd > 2) {//谱峰数大于2时才进行
        //                if (maxInd > i && minInd < i) {//maxInd和minInd都有可能等于i
        //                    maxInd--;
        //                    minInd++;
        //                }
        //                else if (maxInd > i) {//说明minInd == i
        //                    maxInd = i + 1;
        //                }
        //                else if (minInd < i) {//说明maxInd == i
        //                    minInd = i - 1;
        //                }//左右边界做了调整，用意不明，当有maxInd或minInd等于i时，只剩下两根峰；有时会成为一根峰

        //                int npoints = 3;//目前使用三个点的方案
        //                bool maxIntensity = false;//目前使用maxIntensity为假的方案
        //                CentroidPosition centroidPosition = CentroidPosition.gaussian;//位置使用高斯方案
        //                double peakCenterMass, peakIntensity;

        //                //peakIntensity peakCenterMass是将来用于构建Ms1CentroidList中两个参数peakCenterMass peakIntensity
        //                CalcCenterMass(minInd, i, maxInd, profilemz, profileints, out peakIntensity,
        //                    out peakCenterMass, maxIntensity, npoints, centroidPosition);
        //                centermz.Add(peakCenterMass);
        //                centerints.Add(peakIntensity);
        //            }
        //            i = maxIndOri;
        //        }
        //}

        public static T[] SubArray<T>(T[] array, int len)
        {
            if (array.Length <= len)
            {
                return (T[])array.Clone();
            }
            T[] result = new T[len];
            for (int i = 0; i < len; i++)
            {
                result[i] = array[i];
            }
            return result;
        }

        public static void CalcCenterMass(int minInd, int centerInd, int maxInd, List<double> mzs,
                                          List<double> intensities, out double peakIntensity,
                                          out double peakCenterMass, bool maxIntensity, int npoints,
                                          CentroidPosition centroidPosition){
            peakIntensity = 0;
            peakCenterMass = 0;
            for (int j = minInd; j <= maxInd; j++){
                if (maxIntensity){
                    if (intensities[j] > peakIntensity){
                        peakIntensity = intensities[j];
                    }
                }else{
                    peakIntensity += intensities[j];
                }
            }
            peakCenterMass = mzs[centerInd];
            //return;

            if (minInd == maxInd){
                peakCenterMass = mzs[maxInd];
                return;
            }


            if (minInd == centerInd){
                peakCenterMass = Estimate2(mzs[centerInd], mzs[centerInd + 1], intensities[centerInd],
                                           intensities[centerInd + 1]);

                //虚拟峰
                //double tempMzs = 2 * mzs[centerInd] - mzs[centerInd + 1];
                //double tempInt = intensities[centerInd + 1];
                //peakCenterMass = Estimate3(tempMzs, mzs[centerInd], mzs[centerInd + 1],
                //                                tempInt, intensities[centerInd],
                //                                intensities[centerInd + 1]);


                return;
            }


            if (maxInd == centerInd){
                //peakCenterMass = Estimate2(mzs[centerInd - 1], mzs[centerInd], mzs[centerInd - 1],
                //                           mzs[centerInd]);
                peakCenterMass = Estimate2(mzs[centerInd - 1], mzs[centerInd], intensities[centerInd - 1],
                                           intensities[centerInd]);
                //虚拟峰
                //double tempMzs = 2 * mzs[centerInd] - mzs[centerInd - 1];
                //double tempInt = intensities[centerInd - 1];
                //peakCenterMass = Estimate3(mzs[centerInd - 1], mzs[centerInd], tempMzs,
                //                           intensities[centerInd - 1], intensities[centerInd],
                //                          tempInt);

                return;
            }

            if (npoints <= 3){
                switch (centroidPosition){
                    case CentroidPosition.gaussian:
                        peakCenterMass = Estimate3(mzs[centerInd - 1], mzs[centerInd], mzs[centerInd + 1],
                                                   intensities[centerInd - 1], intensities[centerInd],
                                                  intensities[centerInd + 1]);
                        //Console.WriteLine("CentroidPosition: gaussian");
                        break;
                    case CentroidPosition.weightedMean:
                        peakCenterMass =
                            EstimateWeightedMean(new double[] { mzs[centerInd - 1], mzs[centerInd], mzs[centerInd + 1] },
                                                 new double[] { intensities[centerInd - 1], intensities[centerInd], intensities[centerInd + 1] });
                        //Console.WriteLine("CentroidPosition: weightedMean");
                        break;
                    default:
                        throw new Exception("Never get here.");
                }
                return;
            }
        }

        public static double Estimate2(double m1, double m2, double i1, double i2){
            double l1 = Math.Log(i1);
            double l2 = Math.Log(i2);
            return (m1 * l1 + m2 * l2) / (l1 + l2);
        }

        public static double Estimate3(double m1, double m2, double m3, double i1, double i2, double i3)
        {
            double l1 = Math.Log(i1);
            double l2 = Math.Log(i2);
            double l3 = Math.Log(i3);
            return 0.5 * ((l1 - l2) * (m3 * m3 - m1 * m1) - (l1 - l3) * (m2 * m2 - m1 * m1)) /
                   ((l1 - l2) * (m3 - m1) - (l1 - l3) * (m2 - m1));

            //return 0.5 * ((i1 - i2) * (m3 * m3 - m1 * m1) - (i1 - i3) * (m2 * m2 - m1 * m1)) /
            //       ((i1 - i2) * (m3 - m1) - (i1 - i3) * (m2 - m1));
        }

        public static double EstimateWeightedMean(double x1, double x2, double x3, double y1, double y2, double y3)
        {
            double m = 0;
            double w = 0;

            m += x1 * y1;
            w += y1;
            m += x2 * y2;
            w += y2;
            m += x3 * y3;
            w += y3;

            return m / w;
        }

        private static double EstimateWeightedMean(double[] x, double[] y)
        {
            double m = 0;
            double w = 0;
            for (int i = 0; i < x.Length; i++)
            {
                m += x[i] * Math.Log(y[i]);
                w += Math.Log(y[i]);
            }
            return m / w;
        }


        public static void Qwert(double x, double[] a)
        {
            a[0] = 1;
            a[1] = x;
            a[2] = x * x;
        }

        //左零到右零强度谱峰之间的进行中心化
        public static void WindowCentroid(List<double> profilemz, List<double> profileint,
                                          out List<double> centermzs, out List<double> centerints,
                                          Spectrum spec){
            centermzs = new List<double>();
            centerints = new List<double>();
            int ncenter, maxInd, minInd;
            double centerMass = 0, centerInt = 0;

            for (int i = 0; i < profilemz.Count; ++i){
                if (profileint[i] == 0) continue;       // 保证最左边与最右边的峰不为 0
                minInd = i;                 // 记录最左边的峰
                maxInd = i;                 // 记录最右边的峰
                List<int> MaxPeaks = new List<int>();
                ncenter = i;                // 记录强度最高的峰

                // 找到下一个 0 点
                while (maxInd + 1 < profilemz.Count && profileint[maxInd + 1] != 0){
                    ++maxInd;
                    // 强度最高的点作为中心
                    if (profileint[maxInd] > profileint[ncenter])
                        ncenter = maxInd;
                }
                //如果是有效的profile
                if (spec.GetMSLevel() == 1){
                    //一级谱
                    if (IsValidProfileMS1(profilemz, profileint, minInd, maxInd)){
                        //Console.WriteLine("一级谱 Scan：" + spec.scan + 1);
                        for (int j = minInd; j <= maxInd; ++j){
                            if (profileint[j] == profileint[ncenter]){
                                MaxPeaks.Add(j);
                            }
                            // 输出 profile 峰看特点
                            //Console.WriteLine(profilemz[j] + " " + profileint[j]);
                        }
                        ncenter = MaxPeaks[MaxPeaks.Count / 2];//如果强度最高的谱峰不只一个，则取中间的一个
                        //for (int p = 0; p < MaxPeaks.Count(); p++)
                        //{
                        //    centerInt += profileint[p];
                        //}
                        i = maxInd;
                        int npoints = 3;
                        //使用三个点求解
                        //int npoints = maxInd - minInd + 1;
                        CalcCenterMass(minInd, ncenter, maxInd, profilemz, profileint, out centerInt, out centerMass,
                            false, npoints, CentroidPosition.weightedMean);
                        if (!Double.IsNaN(centerMass)){
                            centermzs.Add(centerMass);
                            centerints.Add(centerInt);
                        }
                    }
                }

                if (spec.GetMSLevel() > 1){
                    //二级谱
                    if (IsValidProfileMS2(profilemz, profileint, minInd, maxInd)){
                        //Console.WriteLine("二级谱 Scan：", spec.scan + 1);
                        for (int j = minInd; j <= maxInd; ++j){
                            if (profileint[j] == profileint[ncenter]){
                                MaxPeaks.Add(j);
                            }
                            // 输出 profile 峰看特点
                            //Console.WriteLine(profilemz[j] + " " + profileint[j]);
                        }

                        if(maxInd - minInd > 10){
                            Console.WriteLine("二级谱 Scan：" + spec.scan + 1);
                            for (int j = minInd; j <= maxInd; ++j){
                                Console.WriteLine(profilemz[j] + " " + profileint[j]);
                            }
                            Console.Read();
                        }

                        ncenter = MaxPeaks[MaxPeaks.Count / 2];//如果强度最高的谱峰不只一个，则取中间的一个
                        //for (int p = 0; p < MaxPeaks.Count(); p++)
                        //{
                        //    centerInt += profileint[p];
                        //}
                        i = maxInd;
                        int npoints = 3;
                        //使用三个点求解
                        //int npoints = maxInd - minInd + 1;
                        CalcCenterMass(minInd, ncenter, maxInd, profilemz, profileint, out centerInt, out centerMass,
                            false, npoints, CentroidPosition.weightedMean);
                        if (!Double.IsNaN(centerMass)){
                            centermzs.Add(centerMass);
                            centerints.Add(centerInt);
                        }
                    }
                }
            }
        }

        ////这个方法是基于质量误差的，但是导出谱图的时候，质量精度我们并不知道，所以暂时放弃这种方法。
        //public static void WindowCentroidMS2(List<double> profilemz, List<double> profileint,
        //    out List<double> centermzs, out List<double> centerints)
        //{
        //    centermzs = new List<double>();
        //    centerints = new List<double>();
        //    //left pos and right pos of the window
        //    int ncenter, maxInd, minInd;
        //    double centerMass, centerInt;
        //    for (int i = 0; i < profilemz.Count; ++i)
        //    {
        //        minInd = i;
        //        maxInd = i;
        //        ncenter = i;

        //        List<int> MaxPeaks = new List<int>();
        //        //以MassTorrence为一个窗口，贪心地框谱峰，然后执行中心化操作
        //        while (maxInd + 1 < profilemz.Count && profilemz[maxInd + 1] - profilemz[minInd] <= MS2MassTorrence)
        //        {
        //            ++maxInd;
        //            if (profileint[maxInd] > profileint[ncenter]) ncenter = maxInd;
        //        }
        //        if (IsValidProfileMS2(profilemz, profileint, minInd, maxInd))
        //        {
        //            for (int j = minInd; j <= maxInd; ++j)
        //            {
        //                if (profileint[j] == profileint[ncenter])
        //                {
        //                    MaxPeaks.Add(j);
        //                }
        //            }
        //            ncenter = MaxPeaks[MaxPeaks.Count / 2];//如果强度最高的谱峰不只一个，则取中间的一个
        //            i = maxInd;
        //            int npoints = 3;
        //            //int npoints = maxInd - minInd + 1;
        //            CalcCenterMass(minInd, ncenter, maxInd, profilemz, profileint, out centerInt, out centerMass,
        //                false, npoints, CentroidPosition.gaussian);
        //            if (!Double.IsNaN(centerMass))
        //            {
        //                centermzs.Add(centerMass);
        //                centerints.Add(centerInt);
        //            }
        //        }
        //    }

        //}

        private static bool IsValidProfileMS2(List<double> profilemz, List<double> profileint,
            int minInd, int maxInd){
            //说明只有窗口内只有两根峰
            //if (maxInd - minInd == 1) {
            //    if (profileint[maxInd] == profileint[minInd]) return false;
            //    else return true;
            //}
            //如果包含两根峰以上，就认为是正确的profile
            //if (maxInd - minInd == 1) {
            //    if (profileint[maxInd] == profileint[minInd]) return false;
            //    else return true;
            //} 
            //不去掉一根谱峰的情况，情况比 profilemode 效果好很多
            if (maxInd - minInd >= 0)
            {
                return true;
            }
            return false;
        }

        //public static void WindowCentroidMS1(List<double> profilemz, List<double> profileint,
        //    out List<double> centermzs, out List<double> centerints)
        //{
        //    centermzs = new List<double>();
        //    centerints = new List<double>();
        //    //left pos and right pos of the window
        //    int ncenter, maxInd, minInd;
        //    double centerMass, centerInt;
        //    for (int i = 0; i < profilemz.Count; ++i)
        //    {
        //        maxInd = i;
        //        ncenter = i;

        //        //以MS1MassTorrence为一个窗口，贪心地框谱峰
        //        while (maxInd + 1 < profilemz.Count && profilemz[maxInd + 1] - profilemz[maxInd] <= MS1MassTorrence / 3
        //            && profilemz[maxInd + 1] - profilemz[ncenter] <= MS1MassTorrence)
        //        {
        //            ++maxInd;
        //            if (profileint[maxInd] > profileint[ncenter]) ncenter = maxInd;
        //        }
        //        int endInd = maxInd;
        //        minInd = ncenter;
        //        maxInd = ncenter;

        //        //再以ncenter为中心，加入左右MS2MassTorrence/2的谱峰，然后再找左右的local minima
        //        while (minInd > 0 && profilemz[ncenter] - profilemz[minInd - 1] <= MS2MassTorrence / 2)
        //        {
        //            --minInd;
        //        }
        //        while (minInd > 0 && profilemz[ncenter] - profilemz[minInd - 1] <= MS2MassTorrence / 2
        //            && profileint[minInd - 1] < profileint[minInd])
        //        {
        //            --minInd;
        //        }
        //        while (maxInd + 1 < profilemz.Count && profilemz[maxInd + 1] - profilemz[ncenter] <= MS2MassTorrence / 2)
        //        {
        //            ++maxInd;
        //        }
        //        while (maxInd + 1 < profilemz.Count && profilemz[maxInd + 1] - profilemz[ncenter] <= MS2MassTorrence / 2
        //            && profileint[maxInd + 1] < profileint[maxInd])
        //        {
        //            ++maxInd;
        //        }

        //        if (IsValidProfileMS1(profilemz, profileint, minInd, maxInd))
        //        {
        //            i = endInd;
        //            //这里没有使用npoints > 3时的方案，
        //            //因为拟合二次函数的时候会出现二次函数最高点不在附近的问题
        //            //int npoints = maxInd - minInd + 1;
        //            int npoints = 3;
        //            CalcCenterMass(minInd, ncenter, maxInd, profilemz, profileint, out centerInt, out centerMass,
        //                false, npoints, CentroidPosition.gaussian);
        //            if (!Double.IsNaN(centerMass))
        //            {
        //                centermzs.Add(centerMass);
        //                centerints.Add(centerInt);
        //            }
        //        }
        //    }

        //}

        private static bool IsValidProfileMS1(List<double> profilemz, List<double> profileint, int minInd, int maxInd){
            if (maxInd - minInd >= 0)
                return true;
            return false;
        }
    }
}

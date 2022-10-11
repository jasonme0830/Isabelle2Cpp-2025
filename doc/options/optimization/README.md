## Benchmark

> 结果似乎与环境相关

`--reduce-cond`

**在 macOS 上原生运行**

Comparing bench_merge_sort_ml/ to bench_merge_sort_ml_rc/ (from ./bench)
Benchmark | Time | CPU | Time Old | Time New | CPU Old | CPU New
----------------------------------------------------------|------------------|-------------|--------------|--------------|------------|---------------
[bench_merge_sort_ml/ vs. bench_merge_sort_ml_rc/]256 | -0.0155 | -0.0154 | 134066 | 131982 | 134059 | 131990
[bench_merge_sort_ml/ vs. bench_merge_sort_ml_rc/]512 | -0.0141 | -0.0141 | 297476 | 293275 | 297467 | 293258
[bench_merge_sort_ml/ vs. bench_merge_sort_ml_rc/]1024 | -0.0141 | -0.0141 | 657057 | 647772 | 657024 | 647788
[bench_merge_sort_ml/ vs. bench_merge_sort_ml_rc/]2048 | -0.0134 | -0.0133 | 1443169 | 1423879 | 1443047 | 1423870
[bench_merge_sort_ml/ vs. bench_merge_sort_ml_rc/]4096 | -0.0120 | -0.0120 | 3158510 | 3120569 | 3158477 | 3120542
[bench_merge_sort_ml/ vs. bench_merge_sort_ml_rc/]8192 | -0.0097 | -0.0097 | 6834480 | 6768002 | 6834476 | 6768010
[bench_merge_sort_ml/ vs. bench_merge_sort_ml_rc/]16384 | -0.0068 | -0.0067 | 14754807 | 14654280 | 14753851 | 14654292
OVERALL_GEOMEAN | -0.0122 | -0.0122 | 0 | 0 | 0 | 0

**在 Windows 下的 Linux 子系统中运行**

Comparing bench_merge_sort_ml/ to bench_merge_sort_ml_rc/ (from ./bench)
Benchmark | Time | CPU | Time Old | Time New | CPU Old | CPU New
----------------------------------------------------------|------------------|-------------|--------------|--------------|------------|---------------
[bench_merge_sort_ml/ vs. bench_merge_sort_ml_rc/]256 | +0.0197 | +0.0638 | 83018 | 84653 | 81961 | 87193
[bench_merge_sort_ml/ vs. bench_merge_sort_ml_rc/]512 | +0.0156 | +0.0111 | 185590 | 188483 | 188354 | 190438
[bench_merge_sort_ml/ vs. bench_merge_sort_ml_rc/]1024 | +0.0252 | +0.0000 | 411924 | 422298 | 417150 | 417150
[bench_merge_sort_ml/ vs. bench_merge_sort_ml_rc/]2048 | +0.0052 | -0.0217 | 946271 | 951178 | 962182 | 941265
[bench_merge_sort_ml/ vs. bench_merge_sort_ml_rc/]4096 | -0.0047 | -0.0222 | 2236504 | 2226003 | 2247074 | 2197266
[bench_merge_sort_ml/ vs. bench_merge_sort_ml_rc/]8192 | -0.0023 | -0.0294 | 5289078 | 5276852 | 5312500 | 5156250
[bench_merge_sort_ml/ vs. bench_merge_sort_ml_rc/]16384 | -0.0001 | +0.0000 | 11981192 | 11979800 | 11962891 | 11962891
OVERALL_GEOMEAN | +0.0083 | -0.0002 | 0 | 0 | 0 | 0

`--use-deque`

**在 macOS 上原生运行**

Comparing bench_merge_sort/ to bench_merge_sort_ud/ (from ./bench)
Benchmark | Time | CPU | Time Old | Time New | CPU Old | CPU New
----------------------------------------------------------|------------------|-------------|--------------|--------------|------------|---------------
[bench_merge_sort/ vs. bench_merge_sort_ud/]256 | -0.8264 | -0.8264 | 4745174 | 823909 | 4745216 | 823897
[bench_merge_sort/ vs. bench_merge_sort_ud/]512 | -0.8984 | -0.8984 | 19320880 | 1963414 | 19320667 | 1963468
[bench_merge_sort/ vs. bench_merge_sort_ud/]1024 | -0.9321 | -0.9321 | 79535773 | 5401216 | 79535667 | 5401118
[bench_merge_sort/ vs. bench_merge_sort_ud/]2048 | -0.9482 | -0.9482 | 325841812 | 16866705 | 325841500 | 16866762
[bench_merge_sort/ vs. bench_merge_sort_ud/]4096 | -0.9576 | -0.9576 | 1319001667 | 55903724 | 1319001000 | 55903923
[bench_merge_sort/ vs. bench_merge_sort_ud/]8192 | -0.9636 | -0.9636 | 5353260999 | 194629594 | 5353262000 | 194629250
[bench_merge_sort/ vs. bench_merge_sort_ud/]16384 | -0.9709 | -0.9692 | 24638360250 | 717035000 | 23312904000 | 717034000
OVERALL_GEOMEAN | -0.9401 | -0.9396 | 0 | 0 | 0 | 0

Comparing bench_merge_sort_ml/ to bench_merge_sort_ml_ud/ (from ./bench)
Benchmark | Time | CPU | Time Old | Time New | CPU Old | CPU New
----------------------------------------------------------|------------------|-------------|--------------|--------------|------------|---------------
[bench_merge_sort_ml/ vs. bench_merge_sort_ml_ud/]256 | -0.0937 | -0.0943 | 130318 | 118106 | 130325 | 118032
[bench_merge_sort_ml/ vs. bench_merge_sort_ml_ud/]512 | -0.1416 | -0.1416 | 289478 | 248487 | 289474 | 248491
[bench_merge_sort_ml/ vs. bench_merge_sort_ml_ud/]1024 | -0.1929 | -0.1930 | 640217 | 516698 | 640193 | 516657
[bench_merge_sort_ml/ vs. bench_merge_sort_ml_ud/]2048 | -0.2269 | -0.2269 | 1407870 | 1088426 | 1407827 | 1088422
[bench_merge_sort_ml/ vs. bench_merge_sort_ml_ud/]4096 | -0.2572 | -0.2572 | 3081654 | 2289073 | 3081636 | 2288993
[bench_merge_sort_ml/ vs. bench_merge_sort_ml_ud/]8192 | -0.2811 | -0.2811 | 6675693 | 4798965 | 6675676 | 4798986
[bench_merge_sort_ml/ vs. bench_merge_sort_ml_ud/]16384 | -0.3106 | -0.3106 | 14433078 | 9949815 | 14433163 | 9949700
OVERALL_GEOMEAN | -0.2181 | -0.2182 | 0 | 0 | 0 | 0

Comparing bench_binary_search/ to bench_binary_search_ud/ (from ./bench)
Benchmark | Time | CPU | Time Old | Time New | CPU Old | CPU New
----------------------------------------------------------|------------------|-------------|--------------|--------------|------------|---------------
[bench_binary_search/ vs. bench_binary_search_ud/]256 | -0.8895 | -0.8894 | 5107266 | 564130 | 5107683 | 564777
[bench_binary_search/ vs. bench_binary_search_ud/]512 | -0.9305 | -0.9305 | 20458002 | 1421080 | 20460529 | 1422288
[bench_binary_search/ vs. bench_binary_search_ud/]1024 | -0.9567 | -0.9567 | 86281456 | 3734204 | 86274375 | 3735604
[bench_binary_search/ vs. bench_binary_search_ud/]2048 | -0.9687 | -0.9687 | 348067486 | 10892534 | 348043500 | 10900812
[bench_binary_search/ vs. bench_binary_search_ud/]4096 | -0.9750 | -0.9750 | 1412324401 | 35355267 | 1412327000 | 35367150
[bench_binary_search/ vs. bench_binary_search_ud/]8192 | -0.9783 | -0.9783 | 5665272316 | 123001251 | 5665148000 | 123043333
[bench_binary_search/ vs. bench_binary_search_ud/]16384 | -0.9801 | -0.9801 | 22825153432 | 453564978 | 22824951000 | 453696500
OVERALL_GEOMEAN | -0.9621 | -0.9621 | 0 | 0 | 0 | 0

**在 Windows 下的 Linux 子系统中运行**

Comparing bench_merge_sort/ to bench_merge_sort_ud/ (from ./bench)
Benchmark | Time | CPU | Time Old | Time New | CPU Old | CPU New
----------------------------------------------------------|------------------|-------------|--------------|--------------|------------|---------------
[bench_merge_sort/ vs. bench_merge_sort_ud/]256 | -0.8278 | -0.8240 | 3046097 | 524504 | 3012048 | 530134
[bench_merge_sort/ vs. bench_merge_sort_ud/]512 | -0.8760 | -0.8765 | 12357422 | 1532851 | 12451172 | 1537400
[bench_merge_sort/ vs. bench_merge_sort_ud/]1024 | -0.9103 | -0.9110 | 54355391 | 4876110 | 55397727 | 4928691
[bench_merge_sort/ vs. bench_merge_sort_ud/]2048 | -0.9136 | -0.9112 | 220728900 | 19068268 | 218750000 | 19425676
[bench_merge_sort/ vs. bench_merge_sort_ud/]4096 | -0.9301 | -0.9304 | 1074825000 | 75121630 | 1078125000 | 75000000
[bench_merge_sort/ vs. bench_merge_sort_ud/]8192 | -0.9287 | -0.9288 | 4280347600 | 305260200 | 4281250000 | 304687500
[bench_merge_sort/ vs. bench_merge_sort_ud/]16384 | -0.9339 | -0.9310 | 27986081300 | 1850639400 | 26734375000 | 1843750000
OVERALL_GEOMEAN | -0.9083 | -0.9073 | 0 | 0 | 0 | 0

Comparing bench_merge_sort_ml/ to bench_merge_sort_ml_ud/ (from ./bench)
Benchmark | Time | CPU | Time Old | Time New | CPU Old | CPU New
----------------------------------------------------------|------------------|-------------|--------------|--------------|------------|---------------
[bench_merge_sort_ml/ vs. bench_merge_sort_ml_ud/]256 | +3.1692 | +3.2081 | 83603 | 348557 | 81961 | 344905
[bench_merge_sort_ml/ vs. bench_merge_sort_ml_ud/]512 | +3.3850 | +3.4511 | 185375 | 812869 | 184139 | 819615
[bench_merge_sort_ml/ vs. bench_merge_sort_ml_ud/]1024 | +3.7526 | +3.7455 | 419209 | 1992342 | 419922 | 1992754
[bench_merge_sort_ml/ vs. bench_merge_sort_ml_ud/]2048 | +3.8247 | +3.8506 | 952829 | 4597116 | 941265 | 4565747
[bench_merge_sort_ml/ vs. bench_merge_sort_ml_ud/]4096 | +3.2841 | +3.2391 | 2223470 | 9525677 | 2246094 | 9521484
[bench_merge_sort_ml/ vs. bench_merge_sort_ml_ud/]8192 | +3.5590 | +3.4368 | 5202885 | 23719916 | 5172414 | 22949219
[bench_merge_sort_ml/ vs. bench_merge_sort_ml_ud/]16384 | +3.5702 | +3.4308 | 11908953 | 54426408 | 12207031 | 54086538
OVERALL_GEOMEAN | +3.5009 | +3.4749 | 0 | 0 | 0 | 0

Comparing bench_binary_search/ to bench_binary_search_ud/ (from ./bench)
Benchmark | Time | CPU | Time Old | Time New | CPU Old | CPU New
----------------------------------------------------------|------------------|-------------|--------------|--------------|------------|---------------
[bench_binary_search/ vs. bench_binary_search_ud/]256 | -0.0127 | +0.0065 | 3306383 | 3264360 | 3348214 | 3370098
[bench_binary_search/ vs. bench_binary_search_ud/]512 | -0.0031 | +0.0500 | 12607342 | 12568114 | 12500000 | 13125000
[bench_binary_search/ vs. bench_binary_search_ud/]1024 | +0.0010 | +0.0102 | 49706233 | 49754980 | 51041667 | 51562500
[bench_binary_search/ vs. bench_binary_search_ud/]2048 | +0.0010 | -0.0263 | 197085133 | 197277467 | 197916667 | 192708333
[bench_binary_search/ vs. bench_binary_search_ud/]4096 | -0.0043 | +0.0204 | 792068500 | 788665600 | 765625000 | 781250000
[bench_binary_search/ vs. bench_binary_search_ud/]8192 | +0.0006 | -0.0098 | 3185613300 | 3187677800 | 3203125000 | 3171875000
[bench_binary_search/ vs. bench_binary_search_ud/]16384 | +0.0065 | +0.0036 | 12877192600 | 12961050300 | 12890625000 | 12937500000
OVERALL_GEOMEAN | -0.0016 | +0.0076 | 0 | 0 | 0 | 0

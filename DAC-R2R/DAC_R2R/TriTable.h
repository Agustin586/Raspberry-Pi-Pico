const int Tritable [] = {
2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,
52,54,56,58,60,62,64,66,68,70,72,74,76,78,80,82,84,86,88,90,92,94,96,98,100,
102,104,106,108,110,112,114,116,118,120,122,124,126,128,130,132,134,136,138,140,142,144,146,148,150,
152,154,156,158,160,162,164,166,168,170,172,174,176,178,180,182,184,186,188,190,192,194,196,198,200,
202,204,206,208,210,212,214,216,218,220,222,224,226,228,230,232,234,236,238,240,242,244,246,248,250,
252,254,256,258,260,262,264,266,268,270,272,274,276,278,280,282,284,286,288,290,292,294,296,298,300,
302,304,306,308,310,312,314,316,318,320,322,324,326,328,330,332,334,336,338,340,342,344,346,348,350,
352,354,356,358,360,362,364,366,368,370,372,374,376,378,380,382,384,386,388,390,392,394,396,398,400,
402,404,406,408,410,412,414,416,418,420,422,424,426,428,430,432,434,436,438,440,442,444,446,448,450,
452,454,456,458,460,462,464,466,468,470,472,474,476,478,480,482,484,486,488,490,492,494,496,498,500,
502,504,506,508,510,512,514,516,518,520,522,524,526,528,530,532,534,536,538,540,542,544,546,548,550,
552,554,556,558,560,562,564,566,568,570,572,574,576,578,580,582,584,586,588,590,592,594,596,598,600,
602,604,606,608,610,612,614,616,618,620,622,624,626,628,630,632,634,636,638,640,642,644,646,648,650,
652,654,656,658,660,662,664,666,668,670,672,674,676,678,680,682,684,686,688,690,692,694,696,698,700,
702,704,706,708,710,712,714,716,718,720,722,724,726,728,730,732,734,736,738,740,742,744,746,748,750,
752,754,756,758,760,762,764,766,768,770,772,774,776,778,780,782,784,786,788,790,792,794,796,798,800,
802,804,806,808,810,812,814,816,818,820,822,824,826,828,830,832,834,836,838,840,842,844,846,848,850,
852,854,856,858,860,862,864,866,868,870,872,874,876,878,880,882,884,886,888,890,892,894,896,898,900,
902,904,906,908,910,912,914,916,918,920,922,924,926,928,930,932,934,936,938,940,942,944,946,948,950,
952,954,956,958,960,962,964,966,968,970,972,974,976,978,980,982,984,986,988,990,992,994,996,998,1000,
1002,1004,1006,1008,1010,1012,1014,1016,1018,1020,1022,1024,1026,1028,1030,1032,1034,1036,1038,1040,1042,1044,1046,1048,1050,
1052,1054,1056,1058,1060,1062,1064,1066,1068,1070,1072,1074,1076,1078,1080,1082,1084,1086,1088,1090,1092,1094,1096,1098,1100,
1102,1104,1106,1108,1110,1112,1114,1116,1118,1120,1122,1124,1126,1128,1130,1132,1134,1136,1138,1140,1142,1144,1146,1148,1150,
1152,1154,1156,1158,1160,1162,1164,1166,1168,1170,1172,1174,1176,1178,1180,1182,1184,1186,1188,1190,1192,1194,1196,1198,1200,
1202,1204,1206,1208,1210,1212,1214,1216,1218,1220,1222,1224,1226,1228,1230,1232,1234,1236,1238,1240,1242,1244,1246,1248,1250,
1252,1254,1256,1258,1260,1262,1264,1266,1268,1270,1272,1274,1276,1278,1280,1282,1284,1286,1288,1290,1292,1294,1296,1298,1300,
1302,1304,1306,1308,1310,1312,1314,1316,1318,1320,1322,1324,1326,1328,1330,1332,1334,1336,1338,1340,1342,1344,1346,1348,1350,
1352,1354,1356,1358,1360,1362,1364,1366,1368,1370,1372,1374,1376,1378,1380,1382,1384,1386,1388,1390,1392,1394,1396,1398,1400,
1402,1404,1406,1408,1410,1412,1414,1416,1418,1420,1422,1424,1426,1428,1430,1432,1434,1436,1438,1440,1442,1444,1446,1448,1450,
1452,1454,1456,1458,1460,1462,1464,1466,1468,1470,1472,1474,1476,1478,1480,1482,1484,1486,1488,1490,1492,1494,1496,1498,1500,
1502,1504,1506,1508,1510,1512,1514,1516,1518,1520,1522,1524,1526,1528,1530,1532,1534,1536,1538,1540,1542,1544,1546,1548,1550,
1552,1554,1556,1558,1560,1562,1564,1566,1568,1570,1572,1574,1576,1578,1580,1582,1584,1586,1588,1590,1592,1594,1596,1598,1600,
1602,1604,1606,1608,1610,1612,1614,1616,1618,1620,1622,1624,1626,1628,1630,1632,1634,1636,1638,1640,1642,1644,1646,1648,1650,
1652,1654,1656,1658,1660,1662,1664,1666,1668,1670,1672,1674,1676,1678,1680,1682,1684,1686,1688,1690,1692,1694,1696,1698,1700,
1702,1704,1706,1708,1710,1712,1714,1716,1718,1720,1722,1724,1726,1728,1730,1732,1734,1736,1738,1740,1742,1744,1746,1748,1750,
1752,1754,1756,1758,1760,1762,1764,1766,1768,1770,1772,1774,1776,1778,1780,1782,1784,1786,1788,1790,1792,1794,1796,1798,1800,
1802,1804,1806,1808,1810,1812,1814,1816,1818,1820,1822,1824,1826,1828,1830,1832,1834,1836,1838,1840,1842,1844,1846,1848,1850,
1852,1854,1856,1858,1860,1862,1864,1866,1868,1870,1872,1874,1876,1878,1880,1882,1884,1886,1888,1890,1892,1894,1896,1898,1900,
1902,1904,1906,1908,1910,1912,1914,1916,1918,1920,1922,1924,1926,1928,1930,1932,1934,1936,1938,1940,1942,1944,1946,1948,1950,
1952,1954,1956,1958,1960,1962,1964,1966,1968,1970,1972,1974,1976,1978,1980,1982,1984,1986,1988,1990,1992,1994,1996,1998,2000,
2002,2004,2006,2008,2010,2012,2014,2016,2018,2020,2022,2024,2026,2028,2030,2032,2034,2036,2038,2040,2042,2044,2046,2048,2050,
2052,2054,2056,2058,2060,2062,2064,2066,2068,2070,2072,2074,2076,2078,2080,2082,2084,2086,2088,2090,2092,2094,2096,2098,2100,
2102,2104,2106,2108,2110,2112,2114,2116,2118,2120,2122,2124,2126,2128,2130,2132,2134,2136,2138,2140,2142,2144,2146,2148,2150,
2152,2154,2156,2158,2160,2162,2164,2166,2168,2170,2172,2174,2176,2178,2180,2182,2184,2186,2188,2190,2192,2194,2196,2198,2200,
2202,2204,2206,2208,2210,2212,2214,2216,2218,2220,2222,2224,2226,2228,2230,2232,2234,2236,2238,2240,2242,2244,2246,2248,2250,
2252,2254,2256,2258,2260,2262,2264,2266,2268,2270,2272,2274,2276,2278,2280,2282,2284,2286,2288,2290,2292,2294,2296,2298,2300,
2302,2304,2306,2308,2310,2312,2314,2316,2318,2320,2322,2324,2326,2328,2330,2332,2334,2336,2338,2340,2342,2344,2346,2348,2350,
2352,2354,2356,2358,2360,2362,2364,2366,2368,2370,2372,2374,2376,2378,2380,2382,2384,2386,2388,2390,2392,2394,2396,2398,2400,
2402,2404,2406,2408,2410,2412,2414,2416,2418,2420,2422,2424,2426,2428,2430,2432,2434,2436,2438,2440,2442,2444,2446,2448,2450,
2452,2454,2456,2458,2460,2462,2464,2466,2468,2470,2472,2474,2476,2478,2480,2482,2484,2486,2488,2490,2492,2494,2496,2498,2500,
2502,2504,2506,2508,2510,2512,2514,2516,2518,2520,2522,2524,2526,2528,2530,2532,2534,2536,2538,2540,2542,2544,2546,2548,2550,
2552,2554,2556,2558,2560,2562,2564,2566,2568,2570,2572,2574,2576,2578,2580,2582,2584,2586,2588,2590,2592,2594,2596,2598,2600,
2602,2604,2606,2608,2610,2612,2614,2616,2618,2620,2622,2624,2626,2628,2630,2632,2634,2636,2638,2640,2642,2644,2646,2648,2650,
2652,2654,2656,2658,2660,2662,2664,2666,2668,2670,2672,2674,2676,2678,2680,2682,2684,2686,2688,2690,2692,2694,2696,2698,2700,
2702,2704,2706,2708,2710,2712,2714,2716,2718,2720,2722,2724,2726,2728,2730,2732,2734,2736,2738,2740,2742,2744,2746,2748,2750,
2752,2754,2756,2758,2760,2762,2764,2766,2768,2770,2772,2774,2776,2778,2780,2782,2784,2786,2788,2790,2792,2794,2796,2798,2800,
2802,2804,2806,2808,2810,2812,2814,2816,2818,2820,2822,2824,2826,2828,2830,2832,2834,2836,2838,2840,2842,2844,2846,2848,2850,
2852,2854,2856,2858,2860,2862,2864,2866,2868,2870,2872,2874,2876,2878,2880,2882,2884,2886,2888,2890,2892,2894,2896,2898,2900,
2902,2904,2906,2908,2910,2912,2914,2916,2918,2920,2922,2924,2926,2928,2930,2932,2934,2936,2938,2940,2942,2944,2946,2948,2950,
2952,2954,2956,2958,2960,2962,2964,2966,2968,2970,2972,2974,2976,2978,2980,2982,2984,2986,2988,2990,2992,2994,2996,2998,3000,
3002,3004,3006,3008,3010,3012,3014,3016,3018,3020,3022,3024,3026,3028,3030,3032,3034,3036,3038,3040,3042,3044,3046,3048,3050,
3052,3054,3056,3058,3060,3062,3064,3066,3068,3070,3072,3074,3076,3078,3080,3082,3084,3086,3088,3090,3092,3094,3096,3098,3100,
3102,3104,3106,3108,3110,3112,3114,3116,3118,3120,3122,3124,3126,3128,3130,3132,3134,3136,3138,3140,3142,3144,3146,3148,3150,
3152,3154,3156,3158,3160,3162,3164,3166,3168,3170,3172,3174,3176,3178,3180,3182,3184,3186,3188,3190,3192,3194,3196,3198,3200,
3202,3204,3206,3208,3210,3212,3214,3216,3218,3220,3222,3224,3226,3228,3230,3232,3234,3236,3238,3240,3242,3244,3246,3248,3250,
3252,3254,3256,3258,3260,3262,3264,3266,3268,3270,3272,3274,3276,3278,3280,3282,3284,3286,3288,3290,3292,3294,3296,3298,3300,
3302,3304,3306,3308,3310,3312,3314,3316,3318,3320,3322,3324,3326,3328,3330,3332,3334,3336,3338,3340,3342,3344,3346,3348,3350,
3352,3354,3356,3358,3360,3362,3364,3366,3368,3370,3372,3374,3376,3378,3380,3382,3384,3386,3388,3390,3392,3394,3396,3398,3400,
3402,3404,3406,3408,3410,3412,3414,3416,3418,3420,3422,3424,3426,3428,3430,3432,3434,3436,3438,3440,3442,3444,3446,3448,3450,
3452,3454,3456,3458,3460,3462,3464,3466,3468,3470,3472,3474,3476,3478,3480,3482,3484,3486,3488,3490,3492,3494,3496,3498,3500,
3502,3504,3506,3508,3510,3512,3514,3516,3518,3520,3522,3524,3526,3528,3530,3532,3534,3536,3538,3540,3542,3544,3546,3548,3550,
3552,3554,3556,3558,3560,3562,3564,3566,3568,3570,3572,3574,3576,3578,3580,3582,3584,3586,3588,3590,3592,3594,3596,3598,3600,
3602,3604,3606,3608,3610,3612,3614,3616,3618,3620,3622,3624,3626,3628,3630,3632,3634,3636,3638,3640,3642,3644,3646,3648,3650,
3652,3654,3656,3658,3660,3662,3664,3666,3668,3670,3672,3674,3676,3678,3680,3682,3684,3686,3688,3690,3692,3694,3696,3698,3700,
3702,3704,3706,3708,3710,3712,3714,3716,3718,3720,3722,3724,3726,3728,3730,3732,3734,3736,3738,3740,3742,3744,3746,3748,3750,
3752,3754,3756,3758,3760,3762,3764,3766,3768,3770,3772,3774,3776,3778,3780,3782,3784,3786,3788,3790,3792,3794,3796,3798,3800,
3802,3804,3806,3808,3810,3812,3814,3816,3818,3820,3822,3824,3826,3828,3830,3832,3834,3836,3838,3840,3842,3844,3846,3848,3850,
3852,3854,3856,3858,3860,3862,3864,3866,3868,3870,3872,3874,3876,3878,3880,3882,3884,3886,3888,3890,3892,3894,3896,3898,3900,
3902,3904,3906,3908,3910,3912,3914,3916,3918,3920,3922,3924,3926,3928,3930,3932,3934,3936,3938,3940,3942,3944,3946,3948,3950,
3952,3954,3956,3958,3960,3962,3964,3966,3968,3970,3972,3974,3976,3978,3980,3982,3984,3986,3988,3990,3992,3994,3996,3998,4000,
4002,4004,4006,4008,4010,4012,4014,4016,4018,4020,4022,4024,4026,4028,4030,4032,4034,4036,4038,4040,4042,4044,4046,4048,4050,
4052,4054,4056,4058,4060,4062,4064,4066,4068,4070,4072,4074,4076,4078,4080,4082,4084,4086,4088,4090,4092,4094,4096,4094,4092,
4090,4088,4086,4084,4082,4080,4078,4076,4074,4072,4070,4068,4066,4064,4062,4060,4058,4056,4054,4052,4050,4048,4046,4044,4042,
4040,4038,4036,4034,4032,4030,4028,4026,4024,4022,4020,4018,4016,4014,4012,4010,4008,4006,4004,4002,4000,3998,3996,3994,3992,
3990,3988,3986,3984,3982,3980,3978,3976,3974,3972,3970,3968,3966,3964,3962,3960,3958,3956,3954,3952,3950,3948,3946,3944,3942,
3940,3938,3936,3934,3932,3930,3928,3926,3924,3922,3920,3918,3916,3914,3912,3910,3908,3906,3904,3902,3900,3898,3896,3894,3892,
3890,3888,3886,3884,3882,3880,3878,3876,3874,3872,3870,3868,3866,3864,3862,3860,3858,3856,3854,3852,3850,3848,3846,3844,3842,
3840,3838,3836,3834,3832,3830,3828,3826,3824,3822,3820,3818,3816,3814,3812,3810,3808,3806,3804,3802,3800,3798,3796,3794,3792,
3790,3788,3786,3784,3782,3780,3778,3776,3774,3772,3770,3768,3766,3764,3762,3760,3758,3756,3754,3752,3750,3748,3746,3744,3742,
3740,3738,3736,3734,3732,3730,3728,3726,3724,3722,3720,3718,3716,3714,3712,3710,3708,3706,3704,3702,3700,3698,3696,3694,3692,
3690,3688,3686,3684,3682,3680,3678,3676,3674,3672,3670,3668,3666,3664,3662,3660,3658,3656,3654,3652,3650,3648,3646,3644,3642,
3640,3638,3636,3634,3632,3630,3628,3626,3624,3622,3620,3618,3616,3614,3612,3610,3608,3606,3604,3602,3600,3598,3596,3594,3592,
3590,3588,3586,3584,3582,3580,3578,3576,3574,3572,3570,3568,3566,3564,3562,3560,3558,3556,3554,3552,3550,3548,3546,3544,3542,
3540,3538,3536,3534,3532,3530,3528,3526,3524,3522,3520,3518,3516,3514,3512,3510,3508,3506,3504,3502,3500,3498,3496,3494,3492,
3490,3488,3486,3484,3482,3480,3478,3476,3474,3472,3470,3468,3466,3464,3462,3460,3458,3456,3454,3452,3450,3448,3446,3444,3442,
3440,3438,3436,3434,3432,3430,3428,3426,3424,3422,3420,3418,3416,3414,3412,3410,3408,3406,3404,3402,3400,3398,3396,3394,3392,
3390,3388,3386,3384,3382,3380,3378,3376,3374,3372,3370,3368,3366,3364,3362,3360,3358,3356,3354,3352,3350,3348,3346,3344,3342,
3340,3338,3336,3334,3332,3330,3328,3326,3324,3322,3320,3318,3316,3314,3312,3310,3308,3306,3304,3302,3300,3298,3296,3294,3292,
3290,3288,3286,3284,3282,3280,3278,3276,3274,3272,3270,3268,3266,3264,3262,3260,3258,3256,3254,3252,3250,3248,3246,3244,3242,
3240,3238,3236,3234,3232,3230,3228,3226,3224,3222,3220,3218,3216,3214,3212,3210,3208,3206,3204,3202,3200,3198,3196,3194,3192,
3190,3188,3186,3184,3182,3180,3178,3176,3174,3172,3170,3168,3166,3164,3162,3160,3158,3156,3154,3152,3150,3148,3146,3144,3142,
3140,3138,3136,3134,3132,3130,3128,3126,3124,3122,3120,3118,3116,3114,3112,3110,3108,3106,3104,3102,3100,3098,3096,3094,3092,
3090,3088,3086,3084,3082,3080,3078,3076,3074,3072,3070,3068,3066,3064,3062,3060,3058,3056,3054,3052,3050,3048,3046,3044,3042,
3040,3038,3036,3034,3032,3030,3028,3026,3024,3022,3020,3018,3016,3014,3012,3010,3008,3006,3004,3002,3000,2998,2996,2994,2992,
2990,2988,2986,2984,2982,2980,2978,2976,2974,2972,2970,2968,2966,2964,2962,2960,2958,2956,2954,2952,2950,2948,2946,2944,2942,
2940,2938,2936,2934,2932,2930,2928,2926,2924,2922,2920,2918,2916,2914,2912,2910,2908,2906,2904,2902,2900,2898,2896,2894,2892,
2890,2888,2886,2884,2882,2880,2878,2876,2874,2872,2870,2868,2866,2864,2862,2860,2858,2856,2854,2852,2850,2848,2846,2844,2842,
2840,2838,2836,2834,2832,2830,2828,2826,2824,2822,2820,2818,2816,2814,2812,2810,2808,2806,2804,2802,2800,2798,2796,2794,2792,
2790,2788,2786,2784,2782,2780,2778,2776,2774,2772,2770,2768,2766,2764,2762,2760,2758,2756,2754,2752,2750,2748,2746,2744,2742,
2740,2738,2736,2734,2732,2730,2728,2726,2724,2722,2720,2718,2716,2714,2712,2710,2708,2706,2704,2702,2700,2698,2696,2694,2692,
2690,2688,2686,2684,2682,2680,2678,2676,2674,2672,2670,2668,2666,2664,2662,2660,2658,2656,2654,2652,2650,2648,2646,2644,2642,
2640,2638,2636,2634,2632,2630,2628,2626,2624,2622,2620,2618,2616,2614,2612,2610,2608,2606,2604,2602,2600,2598,2596,2594,2592,
2590,2588,2586,2584,2582,2580,2578,2576,2574,2572,2570,2568,2566,2564,2562,2560,2558,2556,2554,2552,2550,2548,2546,2544,2542,
2540,2538,2536,2534,2532,2530,2528,2526,2524,2522,2520,2518,2516,2514,2512,2510,2508,2506,2504,2502,2500,2498,2496,2494,2492,
2490,2488,2486,2484,2482,2480,2478,2476,2474,2472,2470,2468,2466,2464,2462,2460,2458,2456,2454,2452,2450,2448,2446,2444,2442,
2440,2438,2436,2434,2432,2430,2428,2426,2424,2422,2420,2418,2416,2414,2412,2410,2408,2406,2404,2402,2400,2398,2396,2394,2392,
2390,2388,2386,2384,2382,2380,2378,2376,2374,2372,2370,2368,2366,2364,2362,2360,2358,2356,2354,2352,2350,2348,2346,2344,2342,
2340,2338,2336,2334,2332,2330,2328,2326,2324,2322,2320,2318,2316,2314,2312,2310,2308,2306,2304,2302,2300,2298,2296,2294,2292,
2290,2288,2286,2284,2282,2280,2278,2276,2274,2272,2270,2268,2266,2264,2262,2260,2258,2256,2254,2252,2250,2248,2246,2244,2242,
2240,2238,2236,2234,2232,2230,2228,2226,2224,2222,2220,2218,2216,2214,2212,2210,2208,2206,2204,2202,2200,2198,2196,2194,2192,
2190,2188,2186,2184,2182,2180,2178,2176,2174,2172,2170,2168,2166,2164,2162,2160,2158,2156,2154,2152,2150,2148,2146,2144,2142,
2140,2138,2136,2134,2132,2130,2128,2126,2124,2122,2120,2118,2116,2114,2112,2110,2108,2106,2104,2102,2100,2098,2096,2094,2092,
2090,2088,2086,2084,2082,2080,2078,2076,2074,2072,2070,2068,2066,2064,2062,2060,2058,2056,2054,2052,2050,2048,2046,2044,2042,
2040,2038,2036,2034,2032,2030,2028,2026,2024,2022,2020,2018,2016,2014,2012,2010,2008,2006,2004,2002,2000,1998,1996,1994,1992,
1990,1988,1986,1984,1982,1980,1978,1976,1974,1972,1970,1968,1966,1964,1962,1960,1958,1956,1954,1952,1950,1948,1946,1944,1942,
1940,1938,1936,1934,1932,1930,1928,1926,1924,1922,1920,1918,1916,1914,1912,1910,1908,1906,1904,1902,1900,1898,1896,1894,1892,
1890,1888,1886,1884,1882,1880,1878,1876,1874,1872,1870,1868,1866,1864,1862,1860,1858,1856,1854,1852,1850,1848,1846,1844,1842,
1840,1838,1836,1834,1832,1830,1828,1826,1824,1822,1820,1818,1816,1814,1812,1810,1808,1806,1804,1802,1800,1798,1796,1794,1792,
1790,1788,1786,1784,1782,1780,1778,1776,1774,1772,1770,1768,1766,1764,1762,1760,1758,1756,1754,1752,1750,1748,1746,1744,1742,
1740,1738,1736,1734,1732,1730,1728,1726,1724,1722,1720,1718,1716,1714,1712,1710,1708,1706,1704,1702,1700,1698,1696,1694,1692,
1690,1688,1686,1684,1682,1680,1678,1676,1674,1672,1670,1668,1666,1664,1662,1660,1658,1656,1654,1652,1650,1648,1646,1644,1642,
1640,1638,1636,1634,1632,1630,1628,1626,1624,1622,1620,1618,1616,1614,1612,1610,1608,1606,1604,1602,1600,1598,1596,1594,1592,
1590,1588,1586,1584,1582,1580,1578,1576,1574,1572,1570,1568,1566,1564,1562,1560,1558,1556,1554,1552,1550,1548,1546,1544,1542,
1540,1538,1536,1534,1532,1530,1528,1526,1524,1522,1520,1518,1516,1514,1512,1510,1508,1506,1504,1502,1500,1498,1496,1494,1492,
1490,1488,1486,1484,1482,1480,1478,1476,1474,1472,1470,1468,1466,1464,1462,1460,1458,1456,1454,1452,1450,1448,1446,1444,1442,
1440,1438,1436,1434,1432,1430,1428,1426,1424,1422,1420,1418,1416,1414,1412,1410,1408,1406,1404,1402,1400,1398,1396,1394,1392,
1390,1388,1386,1384,1382,1380,1378,1376,1374,1372,1370,1368,1366,1364,1362,1360,1358,1356,1354,1352,1350,1348,1346,1344,1342,
1340,1338,1336,1334,1332,1330,1328,1326,1324,1322,1320,1318,1316,1314,1312,1310,1308,1306,1304,1302,1300,1298,1296,1294,1292,
1290,1288,1286,1284,1282,1280,1278,1276,1274,1272,1270,1268,1266,1264,1262,1260,1258,1256,1254,1252,1250,1248,1246,1244,1242,
1240,1238,1236,1234,1232,1230,1228,1226,1224,1222,1220,1218,1216,1214,1212,1210,1208,1206,1204,1202,1200,1198,1196,1194,1192,
1190,1188,1186,1184,1182,1180,1178,1176,1174,1172,1170,1168,1166,1164,1162,1160,1158,1156,1154,1152,1150,1148,1146,1144,1142,
1140,1138,1136,1134,1132,1130,1128,1126,1124,1122,1120,1118,1116,1114,1112,1110,1108,1106,1104,1102,1100,1098,1096,1094,1092,
1090,1088,1086,1084,1082,1080,1078,1076,1074,1072,1070,1068,1066,1064,1062,1060,1058,1056,1054,1052,1050,1048,1046,1044,1042,
1040,1038,1036,1034,1032,1030,1028,1026,1024,1022,1020,1018,1016,1014,1012,1010,1008,1006,1004,1002,1000,998,996,994,992,
990,988,986,984,982,980,978,976,974,972,970,968,966,964,962,960,958,956,954,952,950,948,946,944,942,
940,938,936,934,932,930,928,926,924,922,920,918,916,914,912,910,908,906,904,902,900,898,896,894,892,
890,888,886,884,882,880,878,876,874,872,870,868,866,864,862,860,858,856,854,852,850,848,846,844,842,
840,838,836,834,832,830,828,826,824,822,820,818,816,814,812,810,808,806,804,802,800,798,796,794,792,
790,788,786,784,782,780,778,776,774,772,770,768,766,764,762,760,758,756,754,752,750,748,746,744,742,
740,738,736,734,732,730,728,726,724,722,720,718,716,714,712,710,708,706,704,702,700,698,696,694,692,
690,688,686,684,682,680,678,676,674,672,670,668,666,664,662,660,658,656,654,652,650,648,646,644,642,
640,638,636,634,632,630,628,626,624,622,620,618,616,614,612,610,608,606,604,602,600,598,596,594,592,
590,588,586,584,582,580,578,576,574,572,570,568,566,564,562,560,558,556,554,552,550,548,546,544,542,
540,538,536,534,532,530,528,526,524,522,520,518,516,514,512,510,508,506,504,502,500,498,496,494,492,
490,488,486,484,482,480,478,476,474,472,470,468,466,464,462,460,458,456,454,452,450,448,446,444,442,
440,438,436,434,432,430,428,426,424,422,420,418,416,414,412,410,408,406,404,402,400,398,396,394,392,
390,388,386,384,382,380,378,376,374,372,370,368,366,364,362,360,358,356,354,352,350,348,346,344,342,
340,338,336,334,332,330,328,326,324,322,320,318,316,314,312,310,308,306,304,302,300,298,296,294,292,
290,288,286,284,282,280,278,276,274,272,270,268,266,264,262,260,258,256,254,252,250,248,246,244,242,
240,238,236,234,232,230,228,226,224,222,220,218,216,214,212,210,208,206,204,202,200,198,196,194,192,
190,188,186,184,182,180,178,176,174,172,170,168,166,164,162,160,158,156,154,152,150,148,146,144,142,
140,138,136,134,132,130,128,126,124,122,120,118,116,114,112,110,108,106,104,102,100,98,96,94,92,
90,88,86,84,82,80,78,76,74,72,70,68,66,64,62,60,58,56,54,52,50,48,46,44,42,
40,38,36,34,32,30,28,26,24,22,20,18,16,14,12,10,8,6,4,2,0,-2
};
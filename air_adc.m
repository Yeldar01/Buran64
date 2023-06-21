clear all; close all;
vals = [
    0      733 
    0.533  730 
    0.59   730 
    0.775  729 
    0.783  730 
    0.859  730 
    0.600  728 
    0.764  728 
    0.621  727 
    0.974  724 
    0.796  716 
    1.035  705 
    1.102  683 
    1.137  673 
    1.198  650 
    1.336  602 
    1.518  540 
    1.694  480  
    1.804  443 
    1.920  404 
    2.088  347 
    2.302  278 
    2.562  196 
    2.744  140 
    2.88   101 
    3.058  54  
    3.201  20  
    3.214  18  
    3.262  9   
    3.333  1        
    ];

vals47 = [
    0.521 723
    0.600 724
    0.667 723
    0.805 723
    0.898 723
    0.983 714
    1.087 680
    1.219 636
    1.335 595
    1.468 550
    1.617 499
    1.785 442
    1.960 383
    2.213 300
    2.413 235
    2.516 204
    2.746 133
    2.862 100
    2.962 73
    3.073 45
    3.261 5
];

%4.92
vals2 = [
0.001 813
0.128 812
0.183 811
0.35 811
0.391 810 
0.441 810
0.503 809.5
0.591 809
0.626 809
0.717 809
0.967 809
0.991 808
1.012 801
1.028 796
1.043 791
1.093 776
1.114 768
1.188 745
1.251 725
1.316 704
1.355 691
1.461 657
1.505 642
1.599 612
1.669 589
1.754 561
1.853 529
1.981 486
2.056 462
2.247 400
2.418 344
2.526 310
2.630 277
2.727 247
2.795 226 
2.888 197
2.935 185
3.114 130
3.180 112
3.262 90
3.362 64
3.434 47
3.571 17
3.591 13
3.636 5
3.671 1
];

plot(vals(:,1), vals(:,2),'.' );
hold on;
plot(vals2(:,1), vals2(:,2),'.' );
plot(vals47(:,1), vals47(:,2),'.' );

grid on;grid minor;

% 
% % V1 = 4.456;
% V2 = 4.92;
% V1 = 4.7;
% 
% vals = vals47;
% % V1 = V3;
% 
% k = V1/V2;
% 
% pin_v1 = vals(:,1);
% pin_v2 = vals2(:,1);
% 
% pin_adc1 = pin_v1 .* 1023 ./ V1;
% pin_adc2 = pin_v2 .* 1023 ./ V2;
% 
% mcu_adc1 = vals(:,2);
% mcu_adc2 = vals2(:,2);
% 
% figure(1);
% 
% plot(mcu_adc1, pin_v1, '.');
% hold on;
% plot(mcu_adc2, pin_v2, '.');
% grid on;grid minor;
% title('adc vs volts on pin');
% 
% figure(2);
% plot(mcu_adc1, pin_adc1, '.');
% hold on;
% plot(mcu_adc2, pin_adc2, '.');
% grid on;grid minor;
% title('adc vs adc on pin');
% 
% dots = [   
%    0.967 809
%    0.991 808
%    2.247 400
%    2.630 277
%    2.935 185
%    3.262 90
%    3.434 47
%    3.65 0
% ];
% 
% kb = [];
% for i=1:length(dots)-1
%     yx1 = dots(i, :);
%     yx2 = dots(i+1, :);
%     k1 = (yx2(1) - yx1(1)) / (yx2(2) - yx1(2));
%     b1 = yx1(1) - k1 * yx1(2);
%     xr1 = [yx1(2) : -0.01 : yx2(2)];
%     yr1 = xr1 .* k1 + b1;
%     kb = [kb; k1 b1];
% %     plot(xr1, yr1);
% end
% 
% x=[850:-0.2:0.2]';
% Vx = partial_linear_approx(x, kb, dots);
% figure(1);
% plot(x, Vx);
% 
% 
% adc1_scaled = mcu_adc1 ./ k;
% figure(3);
% plot(adc1_scaled, pin_v1, '.');
% hold on;
% plot(mcu_adc1, pin_v1, 'o')
% plot(mcu_adc2, pin_v2, 'x');
% grid on;grid minor;
% 
% Vapp1 = partial_linear_approx(adc1_scaled, kb, dots);
% plot(adc1_scaled, Vapp1, '.');
% 
% Vapp1descaled = Vapp1 .* k;
% plot(mcu_adc1, Vapp1descaled, '.');
% 
% % figure(4);
% diff1 = Vapp1descaled - pin_v1;
% plot(mcu_adc1, diff1, '.');
% grid on;grid minor;

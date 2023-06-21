clear all; close all;

Vdd = 5;

t0 = 273.15;
T25 = 25+t0;
data10k = [    
-55.0   1214600     
-50.0   844390      
-45.0   592430      
-40.0   419380      
-35.0   299480      
-30.0   215670      
-25.0   156410      
-20.0   114660      
-15.0   84510       
-10.0   62927       
-5.0    47077       
0.0    35563       
5.0    27119       
10.0   20860       
15.0   16204       
20.0   12683       
25.0   10000       
30.0   7942        
35.0   6327        
40.0   5074        
45.0   4103        
50.0   3336        
55.0   2724        
60.0   2237        
65.0   1846        
70.0   1530        
75.0   1275        
80.0   1068        
85.0   899.3       
90.0   760.7       
95.0   645.2       
100.0  549.4       
105.0  470.0       
110.0  403.6       
115.0  347.4       
120.0  300.1       
125.0  260.1  
];

data47k = [
-55.0 4879100     
-50.0 3464200     
-45.0 2478000     
-40.0 1785400     
-35.0 1295600     
-30.0 946690      
-25.0 695650      
-20.0 515860      
-15.0 384200      
-10.0 288610      
-5.0  217760      
0.0   165640      
5.0   126880      
10.0  97906       
15.0  76090       
20.0  59531       
25.0  47000       
30.0  36825       
35.0  29315       
40.0  23470       
45.0  18903       
50.0  15302       
55.0  12409       
60.0  10118       
65.0  8316        
70.0  6869        
75.0  5685        
80.0  4725        
85.0  3937        
90.0  3292        
95.0  2770        
100.0 2340        
105.0 1981        
110.0 1683        
115.0 1434        
120.0 1225        
125.0 1050        
];

data = data47k;
Rt = data(:,2);

controlPoints = [
10.0  97906  
40.0  23470        
55.0  12409         
];
correction = -0.2;

% 5.0   126880  
% 40.0  23470        
% 55.0  12409       


T = [controlPoints(:,1)'] + t0;
R = [controlPoints(:,2)'];
L = log(R);
Y = 1./T;
y2 = (Y(2) - Y(1))/(L(2)-L(1));
y3 = (Y(3) - Y(1))/(L(3)-L(1));
C = ((y3-y2)/(L(3)-L(2)))/sum(L);
B = y2 - C * (L(1)^2 + L(1)*L(2)+ L(3)^2);
A = Y(1) - (B + L(1)^2*C)*L(1);
t = 1./(A + B.*log(Rt)+C.*((log(Rt).^3)))+correction;

figure(1);
plot(data(:,2),data(:,1),'.');
hold on;
plot(Rt, t-t0);
ylim([-55 125]);
grid on; grid minor;
xlabel('Ohm');
ylabel('Temperature');

figure(2);
err = data(:,1) - t + t0;
plot(t-t0, err, 'x');
grid on; grid minor;

% air
% Rs = 300000;
% Rgnd = 499000;

% water
Rs = (10000*47000)/(10000+47000);
Rgnd = (20000*100000)/(20000+100000);

Vt = 5*Rgnd./(Rt+Rgnd + Rs);

figure(3)
plot(Rt, Vt);
hold on;

plot([Rt(1) Rt(end)], [3.1 3.1]);
plot([Rt(1) Rt(end)], [1 1]);
xlabel('Ohm');
ylabel('V');
grid on; grid minor;


figure(4);
plot(Vt, t-t0);
xlim([0 5]);
ylim([-40 120]);
ylabel('Temperature');
xlabel('Volts');
hold on;
plot([3.1 3.1], [t(1)-t0 t(end)-t0]);
plot([1 1], [t(1)-t0 t(end)-t0]);
grid on; grid minor;

figure(4);

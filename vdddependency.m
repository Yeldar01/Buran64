clear all;close all;

% mode COM7 BAUD=56000 PARITY=n DATA=8

% type COM7 > water_0-500.txt


files = {
    %'data.txt'
    %'board2.txt'
    %'0.5M+.txt'
    %'1M+.txt'
    
%     '0-5k.txt'
%     '0-5kf.txt'
%     '0-54.txt'
%     '0-5.txt'
%     '0-5-4.txt'
%     '0.5-1.txt'
%     '0.5-1f.txt'
%     '1-1.5.txt'
%     '1-1.5f.txt'
    
    'water_0-500.txt'
    'water_0-500p2.txt'
%     'full2.txt'
%     'full.txt'
    };

data = [];
for i = 1:length(files)
    imported = importdata(files{i});
    data = [data; imported.data];
end

vdd_min = -4.5;
vdd_max = 6.2;
direct_min = -1;
adc_min = -10;
adc_max = 750;

direct_below_k = 0;%1/450;

vdd = data(:,1);
vair_f = data(:,2);
air_adc = data(:,3);
air_v = air_adc .* vdd ./ 1023;
AirTemp = data(:,4);
therm_resistance = data(:,5);
AirDirectADC = data(:,6);
AirDirectVolt = data(:,7);

filtered_index = find(vdd > vdd_min & vdd <= vdd_max);

vdd = vdd(filtered_index);
vair_f = vair_f(filtered_index);
air_adc = air_adc(filtered_index);
air_v = air_adc .* vdd ./ 1023;
AirTemp = AirTemp(filtered_index);
therm_resistance = therm_resistance(filtered_index);
AirDirectADC = AirDirectADC(filtered_index);
AirDirectVolt = AirDirectVolt(filtered_index);

filtered_index = find((air_adc > adc_min) & (air_adc < adc_max));

vdd = vdd(filtered_index);
vair_f = vair_f(filtered_index);
air_adc = air_adc(filtered_index);
air_v = air_adc .* vdd ./ 1023;
AirTemp = AirTemp(filtered_index);
therm_resistance = therm_resistance(filtered_index);
AirDirectADC = AirDirectADC(filtered_index);
AirDirectVolt = AirDirectVolt(filtered_index);

filtered_index = find(AirDirectVolt > direct_min);
vdd = vdd(filtered_index);
vair_f = vair_f(filtered_index);
air_adc = air_adc(filtered_index);
air_v = air_adc .* vdd ./ 1023;
AirTemp = AirTemp(filtered_index);
therm_resistance = therm_resistance(filtered_index);
AirDirectADC = AirDirectADC(filtered_index);
AirDirectVolt = AirDirectVolt(filtered_index);

filtered_index = find(AirDirectVolt > (air_adc.*direct_below_k));
vdd = vdd(filtered_index);
vair_f = vair_f(filtered_index);
air_adc = air_adc(filtered_index);
air_v = air_adc .* vdd ./ 1023;
AirTemp = AirTemp(filtered_index);
therm_resistance = therm_resistance(filtered_index);
AirDirectADC = AirDirectADC(filtered_index);
AirDirectVolt = AirDirectVolt(filtered_index);
%vdd(2:end) = vdd(2:end) - vdd(1:end-1);
%vdd(1) = 0;

numPoints = length(vdd);
numCycles = 2;
colorIndices = mod(0:numPoints-1, numPoints/numCycles) + 1;

%plot3(air_adc, AirDirectVolt, vdd, '.');


scatter3(air_adc, AirDirectVolt, vdd, 50, colorIndices, '.');
hold on;
grid on;grid minor;

view([0 90]);


x = [20:50:720];
vddt = [4.5:0.05:5.2];
[X,VDDT] = meshgrid(x,vddt);
k1 = 4.4407e-05;
b1 = -0.0012652;
k2 = 0.062126;
b2 = 0.74963;
c = 0;%-0.05;
yt = VDDT.*(VDDT.*(k1.*x+k2)+b1.*x+b2) + c;
%yt = VDDT.*((5.3201e-05.*VDDT -0.0013211).*X + (0.10318.*VDDT + 0.6107))+c;
%y = vdd.*((k1.*vdd +b1).*air_adc + (k2.*vdd + b2));
y = vdd.*(vdd.*(k1.*air_adc+k2)+b1.*air_adc+b2) + c;

mesh(X, yt, VDDT, 'FaceAlpha','0.0');  
figure(2);
plot3(air_adc, y-AirDirectVolt, vdd, '.');
grid on;grid minor;
view([0 90]);


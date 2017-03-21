# -*- coding: utf-8 -*-

distancias = {}
raios = {}
luas = {}

UA = 149597870

def writeLuas(estrela):
    
    f_xml.write(f'<group name="{estrela}">\n')
    tx=distancias[estrela][0]/UA
    r=(raios[estrela][0]*10e5)/UA/4600
    f_xml.write(f'<translate X="{tx}" />\n')
    f_xml.write(f'<scale X="{r}" Y="{r}" Z="{r}" />\n')
    
    f_xml.write(f'<models>\n')
    f_xml.write(f'<model file="esfera_1.3d" />\n')
    f_xml.write(f'</models>\n')
    
    for lua in luas.get(estrela, []):
        writeLuas(lua)

    f_xml.write('</group>\n')

for line in open(r'distances_2.txt'):
    split=line.strip().split('\t')
    distancias[split[0]] = (int(split[1]),split[2])
    if split[2] in luas:
        luas[split[2]].append(split[0])
    else:
        luas[split[2]]=[]
        luas[split[2]].append(split[0])

for line in open(r'radius_2.txt'):
    split=line.strip().split('\t')
    raios[split[0]] = (int(split[1]),split[2])

    
f_xml = open(r'../sistema_solar.xml','w+')
f_xml.write('<scene>\n')


writeLuas('Sun')

f_xml.write('</scene>\n')


f_xml.close()


    

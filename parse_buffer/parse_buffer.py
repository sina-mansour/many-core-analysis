#! /usr/bin/python

import os
import sys
import xml.sax


class bufHandler(xml.sax.ContentHandler):
    def __init__(self, outfile):
        self.out = outfile
        self.thr = 0
        self.reported_sz = 0
        self.calculated_sz = 0
        self.real_sz = 0
        self.extra_sz = 0
        self.channel = ''
        self.ch_sz = 0

    def startElement(self, name, attrs):
        if name == 'distributionsSet':
            self.thr = float(attrs['thr'])
            self.reported_sz = int(attrs['sz'])
            self.real_sz = 0
            self.extra_sz = 0
        elif name == 'ch':
            self.channel = (attrs['name'])
            self.ch_sz = int(attrs['sz'])
            self.calculated_sz += self.ch_sz
            if ('__link_to__' in self.channel):
                self.real_sz += self.ch_sz
            else:
                self.extra_sz += self.ch_sz

    def endElement(self, name):
        if name == 'distributionsSet':
            self.write_out()

    def write_out(self):
        self.out.write('\nthroughput: ' + str(self.thr) +
                       '\n\treported buffer size: ' + str(self.reported_sz) +
                       '\n\tcalculated buffer size: ' + str(self.calculated_sz) +
                       '\n\treal buffer size: ' + str(self.real_sz) +
                       '\n\textra buffer size: ' + str(self.extra_sz))


class csvHandler(xml.sax.ContentHandler):
    def __init__(self, csvFile):
        self.out = csvFile
        self.thr = 0
        self.reported_sz = 0
        self.calculated_sz = 0
        self.real_sz = 0
        self.extra_sz = 0
        self.channel = ''
        self.ch_sz = 0

    def startElement(self, name, attrs):
        if name == 'distributionsSet':
            self.thr = float(attrs['thr'])
            self.reported_sz = int(attrs['sz'])
            self.real_sz = 0
            self.extra_sz = 0
        elif name == 'ch':
            self.channel = (attrs['name'])
            self.ch_sz = int(attrs['sz'])
            self.calculated_sz += self.ch_sz
            if ('__link_to__' in self.channel):
                self.real_sz += self.ch_sz
            else:
                self.extra_sz += self.ch_sz

    def endElement(self, name):
        if name == 'distributionsSet':
            self.write_out()

    def write_out(self):
        buf_sz = str(self.real_sz) if (self.real_sz != 0) else ('#'+str(self.calculated_sz))
        self.out.write(str(self.thr) + "#" + buf_sz + ',')


class finalcsvHandler(xml.sax.ContentHandler):
    def __init__(self, csvFile):
        self.out = csvFile
        self.thr = 0
        self.reported_sz = 0
        self.calculated_sz = 0
        self.real_sz = 0
        self.extra_sz = 0
        self.channel = ''
        self.ch_sz = 0

    def startElement(self, name, attrs):
        if name == 'distributionsSet':
            self.thr = float(attrs['thr'])
            self.reported_sz = int(attrs['sz'])
            self.real_sz = 0
            self.extra_sz = 0
        elif name == 'ch':
            self.channel = (attrs['name'])
            self.ch_sz = int(attrs['sz'])
            self.calculated_sz += self.ch_sz
            if ('__link_to__' in self.channel):
                self.real_sz += self.ch_sz
            else:
                self.extra_sz += self.ch_sz

    def endElement(self, name):
        if name == 'storageThroughputTradeOffs':
            self.write_out()

    def write_out(self):
        buf_sz = str(self.real_sz) if (self.real_sz != 0) else (''+str(self.calculated_sz))
        self.out.write(str(self.thr) + "," + buf_sz + ',')


def parse_buffer(xml_file, out_file):
    print ('*** Parsing results from "' + xml_file + '" ***')

    # xmlfile = open(xml_file, 'r')
    outfile = open(out_file + '(temp)', 'w')
    outfile.write('Buffer size calculation for "' + xml_file + '":')

    buf = bufHandler(outfile)
    parser = xml.sax.parse(open(xml_file, 'r'), buf)
    # xml.sax.parse(open(xml_file, 'r'), buf)

    outfile.write('\n')
    outfile.close()
    # xmlfile.close()
    os.rename(out_file + '(temp)', out_file)

    print ('calculations finished -- results saved at "%s"' % (out_file))

def parse_buffer_to_csv(xml_file, csv_file):
    print ('*** Parsing results from "' + xml_file + '" ***')

    # xmlfile = open(xml_file, 'r')
    outfile = open(csv_file, 'a')
    outfile.write(xml_file[xml_file.rfind('/')+1:xml_file.find('.')]+',')

    buf = finalcsvHandler(outfile)
    parser = xml.sax.parse(open(xml_file, 'r'), buf)
    # xml.sax.parse(open(xml_file, 'r'), buf)

    outfile.write('\n')
    outfile.close()
    # xmlfile.close()
    # os.rename(csv_file + '(temp)', csv_file)

    print ('calculations finished -- results saved at "%s"' % (csv_file))


def main():
    xml_filename = sys.argv[1]
    out_filename = xml_filename[:-3] + 'buf'

    parse_buffer(xml_filename, out_filename)


if __name__ == '__main__':
    main()

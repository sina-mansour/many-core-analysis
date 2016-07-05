import os, sys, xml.sax

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
    self.out.write('\nthroughput: '+str(self.thr)+\
                  '\n\treported buffer size: '+str(self.reported_sz)+\
                  '\n\tcalculated buffer size: '+str(self.calculated_sz)+\
                  '\n\treal buffer size: '+str(self.real_sz)+\
                  '\n\textra buffer size: '+str(self.extra_sz))

def main():
  xml_filename = sys.argv[1]
  out_filename = xml_filename[:-3]+'buf(temp)'
  print ('** Parsing XML from "'+xml_filename+'"')

  xmlfile = open(xml_filename, 'r')
  outfile = open(out_filename, 'w')
  outfile.write('Buffer size calculation for "'+xml_filename+'":')

  buf = bufHandler(outfile)
  parser = xml.sax.parse(open(xml_filename, 'r'), buf)

  outfile.write('\n')
  outfile.close()
  xmlfile.close()
  os.rename(out_filename, out_filename[:-6])

  print ('calculations finished -- results saved at "%s"' %(out_filename[:-6]))

if __name__ == '__main__': main()
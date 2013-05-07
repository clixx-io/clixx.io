
# A dictionary of all devices
devices = {}

# 
def autoload():
  """ Automatically loads device map
  """
  devices = { 'Digital-1': 'o'
            }
# Device_lo is the csv log
device_log = something

def loop:

  line = []
  for p in inputpins():

    v = p.readValue()

    line.append(v)

  device_log.outputline(line)


  
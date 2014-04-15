# Allow access to command-line arguments
import sys,os, glob
 
# Import the core and GUI elements of Qt
from PySide.QtCore import *
from PySide.QtGui import *

def supported_image_extensions():
    ''' Get the image file extensions that can be read. '''
    formats = QImageReader().supportedImageFormats()
    # Convert the QByteArrays to strings
    return [str(fmt) for fmt in formats]
    
    
class ImageFileList(QListWidget):
    ''' A specialized QListWidget that displays the
        list of all image files in a given directory. '''
 
    def __init__(self, dirpath, parent=None):
        QListWidget.__init__(self, parent)
        self.setDirpath(dirpath)
 
 
    def setDirpath(self, dirpath):
        ''' Set the current image directory and refresh the list. '''
        self._dirpath = dirpath
        self._populate()
 
 
    def _images(self):
        ''' Return a list of filenames of all
            supported images in self._dirpath. '''
 
        # Start with an empty list
        images = []
 
        # Find the matching files for each valid
        # extension and add them to the images list
        for extension in supported_image_extensions():
            pattern = os.path.join(self._dirpath,
                                   '*.%s' % extension)
        #    images.extend(glob(pattern))
 
        return glob.glob('../webinterface/media/icons/*')
        # return images
 
 
    def _populate(self):
        ''' Fill the list with images from the
            current directory in self._dirpath. '''
 
        # In case we're repopulating, clear the list
        self.clear()
 
        # Create a list item for each image file,
        # setting the text and icon appropriately
        for image in self._images():
            item = QListWidgetItem(self)
            item.setText(image)
            item.setIcon(QIcon(image))
            
if __name__ == '__main__':
    # The app doesn't receive sys.argv, because we're using
    # sys.argv[1] to receive the image directory
    app = QApplication([])
 
    # Create a window, set its size, and give it a layout
    win = QWidget()
    win.setWindowTitle('Project List')
    win.setMinimumSize(600, 400)
    layout = QVBoxLayout()
    win.setLayout(layout)
 
    # Create one of our ImageFileList objects using the image
    # directory passed in from the command line
    lst = ImageFileList('1', win)
 
    layout.addWidget(lst)
 
    entry = QLineEdit(win)
 
    layout.addWidget(entry)
 
    def on_item_changed(curr, prev):
        entry.setText(curr.text())
 
    lst.currentItemChanged.connect(on_item_changed)
 
    win.show()
    app.exec_()            

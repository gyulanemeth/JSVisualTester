class Browser:
	def __init__ (self, testRunner, name):
		self.Message ('browser init (' + name + ')')
		self.testRunner = testRunner
		self.name = name

	def GetName (self):
		return self.name
	
	def Open (self, width, height, offsets):
		self.Message ('browser open (' + str (width) + ', ' + str (height) + ', ' + str (offsets) + ')')
		self.testRunner.KeyPress (0x52, 0x5B)
		self.testRunner.Sleep (1000)
		self.testRunner.TextWrite (self.name)
		self.testRunner.KeyPress (0x0D, 0)
		self.testRunner.Sleep (5000)
		self.testRunner.MoveWindow (0, 0, width, height);
		self.testRunner.Sleep (1000)
		self.rect = self.testRunner.GetClientRect ()
		self.rect[0] = self.rect[0] + offsets[0]
		self.rect[1] = self.rect[1] + offsets[1]
		self.rect[2] = self.rect[2] - offsets[2]
		self.rect[3] = self.rect[3] - offsets[3]

	def Close (self):
		self.Message ('browser close')
		self.testRunner.KeyPress (0x73, 0x12)
		self.testRunner.Sleep (2000)	

	def SetURL (self, url):
		self.Message ('browser set url (' + url + ')')
		self.testRunner.KeyPress (0x4C, 0x11)
		self.testRunner.Sleep (1000)
		self.testRunner.TextWrite (url)
		self.testRunner.KeyPress (0x0D, 0);
		self.testRunner.Sleep (3000)

	def Click (self, x, y):
		self.Message ('browser click (' + str (x) + ', ' + str (y) + ')')
		self.testRunner.MouseClick (self.rect[0] + x, self.rect[1] + y)
		self.testRunner.Sleep (1000)

	def DragDrop (self, x, y, newX, newY):
		self.Message ('browser drag and drop (' + str (x) + ', ' + str (y) + ', ' + str (newX) + ', ' + str (newY) + ')')
		self.testRunner.MouseDragDrop (self.rect[0] + x, self.rect[1] + y, self.rect[0] + newX, self.rect[1] + newY, 100)
		self.testRunner.Sleep (1000)

	def KeyPress (self, keyCode, scanCode):
		self.Message ('browser key press (' + str (keyCode) + ', ' + str (scanCode) + ')')
		self.testRunner.KeyPress (keyCode, scanCode)
		self.testRunner.Sleep (1000)

	def TextWrite (self, text):
		self.Message ('browser text write (' + text + ')')
		self.testRunner.TextWrite (text)
		self.testRunner.Sleep (1000)

	def Capture (self, name):
		self.Message ('browser capture (' + name + ')')
		self.testRunner.CaptureRect (self.rect, self.name + '_' + name + '.png');

	def Message (self, message):
		print (message)

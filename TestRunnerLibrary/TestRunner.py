import os
import shutil
import ctypes

from ctypes import *
from ctypes.wintypes import RECT

import HTMLTableWriter

class TestRunner:
	def __init__ (self):
		currentDir = os.path.dirname (os.path.abspath (__file__))
		path = os.path.join (currentDir, 'TestRunner.dll')
		self.testRunner = cdll.LoadLibrary (path)
		self.counter = 0
		self.referencesFolder = 'references'
		self.resultsFolder = 'results'
		self.differencesFolder = os.path.join (self.resultsFolder, 'differences')
		self.currentFolder = os.path.join (self.resultsFolder, 'current')
		if not os.path.exists (self.referencesFolder):
			os.mkdir (self.referencesFolder)
		if os.path.exists (self.resultsFolder):
			shutil.rmtree (self.resultsFolder)
		os.mkdir (self.resultsFolder)
		os.mkdir (self.differencesFolder)
		os.mkdir (self.currentFolder)
	
	def __del__ (self):
		del self.testRunner

	def Sleep (self, milliSeconds):
		self.testRunner.TRSleep (milliSeconds)

	def MoveWindow (self, left, top, right, bottom):
		self.testRunner.TRMoveWindow (left, top, right, bottom)

	def GetClientRect (self):
		rect = RECT ()
		self.testRunner.TRGetForegroundClientRect (ctypes.addressof (rect))
		return [rect.left, rect.top, rect.right, rect.bottom]
		
	def KeyPress (self, keyCode, scanCode):
		self.testRunner.TRKeyPress (keyCode, scanCode)

	def TextWrite (self, text):
		self.testRunner.TRTextWrite (ctypes.c_wchar_p (text))

	def MouseClick (self, x, y):
		self.testRunner.TRMouseClick (x, y)

	def MouseDoubleClick (self, x, y):
		self.testRunner.TRMouseDoubleClick (x, y)

	def MouseDragDrop (self, x, y, newX, newY, milliSeconds):
		self.testRunner.TRMouseDragDrop (x, y, newX, newY, milliSeconds)

	def CaptureScreen (self, fileName):
		self.testRunner.TRCaptureScreen (ctypes.c_wchar_p (self.GetNextCaptureFileName (fileName)))
	
	def CaptureClient (self, fileName):
		self.testRunner.TRCaptureClient (ctypes.c_wchar_p (self.GetNextCaptureFileName (fileName)))
	
	def CaptureRect (self, rect, fileName):
		self.testRunner.TRCaptureRect (rect[0], rect[1], rect[2], rect[3], ctypes.c_wchar_p (self.GetNextCaptureFileName (fileName)))

	def GetNextCaptureFileName (self, fileName):
		self.counter = self.counter + 1
		formatString = '{0:05d}'
		resultFileName = formatString.format (self.counter) + '_' + fileName
		return os.path.join (self.currentFolder, resultFileName)

	def GenerateResult (self):
		resultFilePath = os.path.join (self.resultsFolder, 'result.html')
		htmlTableWriter = HTMLTableWriter.HTMLTableWriter (resultFilePath, 'Result')
		htmlTableWriter.WriteHeader ();
		htmlTableWriter.WriteTableHeader (['file name', 'image 1', 'image 2', 'diff image', 'diff count', 'result']);
		
		for fileName in os.listdir (self.referencesFolder):
			filePath1 = os.path.join (self.referencesFolder, fileName)
			filePath2 = os.path.join (self.currentFolder, fileName)
			filePath3 = os.path.join (self.differencesFolder, fileName)
			column1 = '<a href="../' + self.referencesFolder + '/' + fileName + '">image</a>';
			column2 = '<a href="../' + self.currentFolder + '/' + fileName + '">image</a>';
			column3 = '<a href="../' + self.differencesFolder + '/' + fileName + '">image</a>';

			if not os.path.exists (filePath1) or not os.path.exists (filePath2):
				result = '<span class="failed">not exists</span>'
				htmlTableWriter.WriteTableRow ([fileName, column1, column2, '-', '-', result]);
				continue
				
			diffRes = self.testRunner.TRCompareImages (ctypes.c_wchar_p (filePath1), ctypes.c_wchar_p (filePath2), ctypes.c_wchar_p (filePath3))
			if diffRes == -1:
				result = '<span class="failed">not equal size</span>'
				htmlTableWriter.WriteTableRow ([fileName, column1, column2, '-', '-', result]);
				continue

			if diffRes > 0:
				result = '<span class="failed">failed</span>'
				htmlTableWriter.WriteTableRow ([fileName, column1, column2, column3, str (diffRes), result]);
				continue

			result = '<span class="succeeded">succeeded</span>'
			htmlTableWriter.WriteTableRow ([fileName, column1, column2, '-', '-', result]);
		
		htmlTableWriter.WriteTableFooter ();
		htmlTableWriter.WriteFooter ();

import os
import sys
import time

parentDir = os.path.dirname (os.path.dirname (os.path.abspath (__file__)))
sys.path.insert (0, parentDir) 
from TestRunnerLibrary import TestRunner
from TestRunnerLibrary import Browser

def Main (argv):
	currentPath = os.path.dirname (os.path.abspath (__file__))
	os.chdir (currentPath)

	# initialize TestRunner library
	testRunner = TestRunner.TestRunner ()
	
	# create a Browser instance with the name of application (firefox.exe)
	browser = Browser.Browser (testRunner, 'firefox')
	
	# opens the browser with the given size
	# the second parameter defines the border sizes of the browser
	browser.Open (800, 600, [2, 114, 2, 2])

	# opens a URL in browser
	browser.SetURL (os.path.abspath ('exampletest.html'))

	# events and captures
	browser.Click (20, 20)
	browser.Capture ('example');
	browser.DragDrop (30, 30, 50, 50)
	browser.Capture ('example');
	browser.KeyPress (0x41, 0)
	browser.Capture ('example');

	# close the browser
	browser.Close ()
	
	# generates the result in results folder
	# compares captures with the ones in the references folder
	# generates result.html with the results
	testRunner.GenerateResult ()
	
	exit (0)
	
Main (sys.argv)

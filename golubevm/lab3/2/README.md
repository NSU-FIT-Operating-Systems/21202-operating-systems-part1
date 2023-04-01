# Lab3-2: Different hardlinks to same program
---
To build program use make
---
  	mkd:
  		mkd dirname
  		Creates new directory.
  	lsd:
  		lsd dirname
  		Prints all files contained in quered directory.
  	rmd:
  		rmd dirname
  		Remove directory.
  	crt:
  		crt filename
  		Creates a regular file.
  	fprint:
  		fprint filename
  		Print all data contained in file to stdout. If you
  		apply this command to symlink it will show content of file
  		which path link stores.
  	rmf:
  		rmf filename
  		Unlink file. If there no more hardlinks, file will be deleted.
  	smlk:
  		smlk path
  		Creates symlink to file.
  	smlprint:
  		smlprint filename
  		Prints path stored in symlink.
  	hrdlink:
  		hrdlink filename
  		Creates hard link. To remove hardlink use rmf.
  	fstt:
  		fstt filename
  		Shows file statistics.
  	chmd:
  		chmd filename
  		Change current file permissions.

import subprocess
from PIL import Image,ImageDraw,ImageFont
import numpy as np
import math
import tempfile
import os

class ImageProcessor():
    
    def __init__(self,fontSize):
        self.letterHeight=24
        self.fontSize = fontSize
        #Do not touch these values
        self.fb_width = 1280
        self.fb_height = 800
        self.maxChar = 950
        # self.font = ImageFont.load_default(self.fontSize)
        # self.font = ImageFont.truetype("arial.ttf", self.fontSize)

    def IpFinder(self):
        cmd = ['hostname','-I']
        # IP address tester
        ipAdd = -1
        try:
            proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            o, e = proc.communicate()

            error = e.decode('ascii')
            ipAdd=o.decode('ascii').strip()
    
        except:
            print("An error occured")
        if(ipAdd != -1):
            return ipAdd


    def imageProcessor(self,path):
        im = Image.open(path)
        im_w,im_h=im.size
        
        
        ratio=self.fb_height/im_h
        resized_im_w =int(im_w*ratio)
        resized_im = im.resize((resized_im_w,  self.fb_height))
        resized_im_w,resized_im_h = resized_im.size
        
        print(im_w)
        print(im_h)
        print(resized_im_w)
        print(resized_im_h)
        print("\n"+str(ratio))
        
        
        iar = np.asarray(resized_im)
        # Convert to RGB
        rgbOnly = iar[:, :, :3]
        newImage = Image.fromarray(rgbOnly, 'RGB')
        imageArray = np.array(newImage)
        return imageArray
    

    def createLetterImage(self,letter):
        spacing=4
        letterWidth=  math.floor(self.font.getlength(letter))
        letterXSpacing= letterWidth + spacing
        
        
        # Create a blank image for the letter (40x80)
        letter_image = Image.new("RGB", (letterWidth, self.letterHeight), "black")
        draw = ImageDraw.Draw(letter_image)
        
        # Draw the letter in white
        draw.text((0, 0), letter, fill="white", spacing=spacing,font=self.font)
        

        return letter_image, letterWidth,letterXSpacing

    def createImage(self,word,fontSize,xInitPos,yInitPos):
        
        image = Image.new("RGB", (self.fb_width, self.fb_height), "black")
        draw = ImageDraw.Draw(image)
        x_position = xInitPos
        y_position = yInitPos
        
        for line in word.split('\n'):
            
            x_position = xInitPos  # Reset x_position for each line
            y_position += self.letterHeight  # Move to the next line position

            for letter in line:
                #  Check if the next line would go beyond the specified height
                if (y_position + self.letterHeight) >= self.fb_height - yInitPos:
                    break
                else:
                    letter_image, letterWidth,letterXSpacing= self.createLetterImage(letter)
                    image.paste(letter_image, (x_position, y_position))
                    x_position += letterXSpacing  # Move to the next letter position
                    if((x_position + letterWidth) >=  self.fb_width-xInitPos):
                        y_position += self.letterHeight
                        x_position=xInitPos
        imageArray = np.array(image)
        return image,imageArray
    
    def transmitArrayToCframeBufferHandler(self, imageArray):
        # Assume imageArray is a 3D NumPy array with shape (height, width, 3)
        height, width, _ = imageArray.shape
        print(f"Original image size - height:{height}, width: {width}, channels: {_}")

        # Rotate the imageArray 90° clockwise
        rotated_imageArray = np.rot90(imageArray, k=-1, axes=(1, 0))
        # Rotate the imageArray 90° clockwise
        rotated_imageArray1 = np.rot90(rotated_imageArray, k=-1, axes=(1, 0))
        # # Rotate the imageArray 90° clockwise
        rotated_imageArray2 = np.rot90(rotated_imageArray1, k=-1, axes=(0, 1))
        # rotated_imageArray2 = np.rot90(rotated_imageArray1, k=-1, axes=(0, 1))
        height, width, _ = rotated_imageArray1.shape
        print(f"Rotated image size - height:{height}, width: {width}, channels: {_}")
        rotated_imageArray = np.fliplr(rotated_imageArray1)

        # Flatten the RGB values into a 1D array
        flat_array = rotated_imageArray.reshape(-1)
        print(f"Flat array: {flat_array}")

        # Create a temporary file to store the RGB values
        with tempfile.NamedTemporaryFile(mode='w+', delete=False) as temp_file:
            # Write height, width, and RGB values to the file
            temp_file.write(f"{height} {width}\n")
            temp_file.write(' '.join(map(str, flat_array)))

        try:
            # Call the C executable using subprocess with the file path as an argument
            subprocess.run(['./frameBufferHandler', temp_file.name], text=True)
        finally:
            # Clean up: Remove the temporary file
            os.remove(temp_file.name)



fontSize = 18
improc=ImageProcessor(fontSize)

# #Should IP finder be here?
ip = improc.IpFinder()

text= f"IP ADDRESS: {ip}\n"

# impath="imageProc/yasu.jpeg"
# displayArr=improc.imageProcessor(impath)
displayArr=improc.imageProcessor("yasu.jpeg")

# (display,displayArr) = improc.createImage(text,fontSize,20,20)
# improc.transmitArrayToCframeBufferHandler(displayArr)



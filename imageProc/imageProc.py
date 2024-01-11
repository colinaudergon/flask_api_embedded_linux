import subprocess
from PIL import Image,ImageDraw,ImageFont
import numpy as np
import math

class ImageProcessor():
    
    def __init__(self,fontSize):
        self.letterHeight=24
        self.fontSize = fontSize
        # self.font = ImageFont.load_default(self.fontSize)
        self.font = ImageFont.truetype("arial.ttf", self.fontSize)

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


    def imageProcessor(path):
        i = Image.open(path)
        iar = np.asarray(i)
        # Convert to RGB
        rgbOnly = iar[:, :, :3]
        newImage = Image.fromarray(rgbOnly, 'RGB')
        return newImage


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

    def createImage(self,word,width,height,fontSize,xInitPos,yInitPos):
        
        # global letterHeight
        # font = ImageFont.load_default(fontSize)
        
        image = Image.new("RGB", (width, height), "black")
        draw = ImageDraw.Draw(image)
        x_position = xInitPos
        y_position = yInitPos
        
        for line in word.split('\n'):
            
            x_position = xInitPos  # Reset x_position for each line
            y_position += self.letterHeight  # Move to the next line position

            for letter in line:
                #  Check if the next line would go beyond the specified height
                if (y_position + self.letterHeight) >= height - yInitPos:
                    break
                else:
                    letter_image, letterWidth,letterXSpacing= self.createLetterImage(letter)
                    image.paste(letter_image, (x_position, y_position))
                    x_position += letterXSpacing  # Move to the next letter position
                    if((x_position + letterWidth) >=  width-xInitPos):
                        y_position += self.letterHeight
                        x_position=xInitPos
        imageArray = np.array(image)
        return image,imageArray



    def transmitArrayToCframeBufferHandler(self, imageArray):
        # Assume imageArray is a 3D NumPy array with shape (height, width, 3)
        height, width, _ = imageArray.shape

        # Flatten the RGB values into a 1D array
        flat_array = imageArray.reshape(-1)

        # Convert the flat array to a string with newline characters
        rgb_str = '\n'.join(map(str, flat_array))

        # Combine height, width, and RGB values into the final input string
        input_str = f"{height} {width}\n{rgb_str}\n"

        # Call the C executable using subprocess
        subprocess.run(['./frameBufferHandler'], input=input_str, text=True)




fontSize = 18
improc=ImageProcessor(fontSize)

# #Should IP finder be here?
ip = improc.IpFinder()

# text= f"IP ADDRESS: {ip}\n"

# # ~980 character max
(display,displayArr) = improc.createImage(ip,640,480,fontSize,10,10)
improc.transmitArrayToCframeBufferHandler(displayArr)
# display.save("ipaddress.png")
print(displayArr.ndim)


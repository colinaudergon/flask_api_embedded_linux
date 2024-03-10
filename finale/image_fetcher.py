import os
import requests

def imageFetcher(api_url, save_directory):
    response = requests.get(api_url)
    data = response.json()

    for file_info in data:
        file_name = file_info["name"]
        download_url = file_info["download_url"]

        # Extract game name and extension
        game_name, extension = os.path.splitext(file_name)

        # Check if the file is a PNG image
        if extension.lower() == '.png':
            # Download the image
            image_response = requests.get(download_url)
            image_data = image_response.content

            # Save the image with the game name in the 'images' directory
            save_path = os.path.join(save_directory, f"{game_name}.png")
            with open(save_path, 'wb') as image_file:
                image_file.write(image_data)

if __name__ == "__main__":
    github_api_url = 'https://api.github.com/repos/retrobrews/nes-games/contents'
    save_directory = os.path.join(os.getcwd(), 'images')
    # Ensure the 'images' directory exists
    os.makedirs(save_directory, exist_ok=True)

    imageFetcher(github_api_url, save_directory)

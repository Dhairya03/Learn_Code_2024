import readline from 'readline';

const readlineInterface = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

async function getTumblrData(blogName: string, start: number, end: number) {
    const apiUrl = `https://${blogName}.tumblr.com/api/read/json?type=photo&num=${end - start + 1}&start=${start - 1}`;
    try {
        const rawResponse = await fetch(apiUrl);
        const textResponse = await rawResponse.text(); 

        const jsonpCallback = (data: any) => {
            console.log(data); 
        };

        const response=eval(textResponse);

        const jsonText = response.data.replace(/^.*?\{/, '{').replace(/\};?$/, '}');
        const jsonData = JSON.parse(jsonText);
        
        console.log(`title: ${jsonData.tumblelog.title}`);
        console.log(`name: ${jsonData.tumblelog.name}`);
        console.log(`description: ${jsonData.tumblelog.description}`);
        console.log(`no of post: ${jsonData["posts-total"]}\n`);
        
        jsonData.posts.forEach((post: any, index: number) => {
            console.log(`${start + index}.`);
            if (post.photos) {
                post.photos.forEach((photo: any) => {
                    for (const key in photo) {
                        if (key.startsWith("photo-url")) {
                            console.log(`   ${photo[key]}`);
                        }
                    }
                });
            } else if (post["photo-url-1280"]) {
                console.log(`   ${post["photo-url-1280"]}`);
            }
        });
    } catch (error) {
        if (error instanceof Error) {
            console.error("Error fetching data:", error.message);
        } else {
            console.error("An unknown error occurred:", error);
        }
    }
}

readlineInterface.question("Enter the Tumblr blog name: ", (blogName) => {
    readlineInterface.question("Enter the range (start-end): ", (postRange) => {
        try {
            const [start, end] = postRange.split('-').map(Number);
            if (isNaN(start) || isNaN(end) || start < 1 || end < start) {
                throw new Error("Invalid range values.");
            }
            getTumblrData(blogName, start, end).finally(() => readlineInterface.close());
        }catch (error) {
            if (error instanceof Error) {
                console.error("Error fetching data:", error.message);
            } else {
                console.error("An unknown error occurred:", error);
            }
        }
    });
});

const express = require('express');
const bodyParser = require('body-parser');
const fs = require('fs');
const jpeg = require('jpeg-js');
const cors = require('cors');

const app = express();
app.use(cors());

const clients = new Map();

const getUniqueID = () => {
    return Date.now().toString(36) + Math.random().toString(36).substring(2);
}

function rgb565ToJpg(width, height, imageData) {
    const rgbaBuffer = Buffer.alloc(width * height * 4);
    for (let i = 0, j = 0; i < imageData.length; i += 2) {
        const rgb565 = (imageData[i] << 8) | imageData[i + 1];

        const r = ((rgb565 >> 11) & 0x1F) << 3;
        const g = ((rgb565 >> 5) & 0x3F) << 2;
        const b = (rgb565 & 0x1F) << 3;

        rgbaBuffer[j++] = r;
        rgbaBuffer[j++] = g;
        rgbaBuffer[j++] = b;
        rgbaBuffer[j++] = 0xFF; // Alpha channel (fully opaque)
    }

    const ret = jpeg.encode({
        data: rgbaBuffer,
        width,
        height
    }, 75)

    return ret.data;
}

// 解析 application/json
app.use(bodyParser.raw({ limit: '50mb', type: ["image/rgb565", "image/jpeg"] }));

// 接收 RGB565 格式图像数据
app.post('/rgb565/upload', (req, res) => {
    // 检查请求体中是否包含图像数据
    if (!req.body) {
        return res.status(400).json({ error: 'No image data provided' });
    }

    let width = Number(req.headers['x-width']);
    let height = Number(req.headers['x-height']);

    /**
     * @type {Buffer}
     */
    const buffer = req.body;
    const jpgBuffer = rgb565ToJpg(width, height, buffer);

    console.log("upload raw size:", buffer.length, "jpg size:", jpgBuffer.length);

    // 将图像数据保存为文件
    const filename = 'uploads/rgb565_' + Date.now() + '.jpg'; // 定义保存的文件名
    fs.writeFile(filename, jpgBuffer, (err) => {
        if (err) {
            return res.status(500).json({ error: 'Failed to save image' });
        }
        console.log('Image saved successfully...');
        res.json({ success: true });
    });
});

app.post('/jpg/upload', (req, res) => {
    // 检查请求体中是否包含图像数据
    if (!req.body) {
        return res.status(400).json({ error: 'No image data provided' });
    }

    let width = Number(req.headers['x-width']);
    let height = Number(req.headers['x-height']);

    /**
     * @type {Buffer}
     */
    const buffer = req.body;

    console.log("jpg size:", buffer.length, "width:", width, "height:", height);

    // 将图像数据保存为文件
    const filename = 'uploads/jpg_' + Date.now() + '.jpg';
    fs.writeFile(filename, buffer, (err) => {
        if (err) {
            return res.status(500).json({ error: 'Failed to save image' });
        }
        console.log('Image saved successfully...');
        res.json({ success: true });
    });
});

app.post('/push/data', (req, res) => {
    // 检查请求体中是否包含图像数据
    if (!req.body) {
        return res.status(400).json({ error: 'No image data provided' });
    }

    /**
     * @type {Buffer}
     */
    const buffer = req.body;
    const b64 = buffer.toString("base64");

    clients.forEach((sendEvent) => {
        sendEvent(b64);
    });

    res.json({ success: true });
})

app.get("/pull/data", (req, res) => {
    // 设置响应头，指明这是一个SSE连接
    res.setHeader('Content-Type', 'text/event-stream');
    res.setHeader('Cache-Control', 'no-cache');
    res.setHeader('Connection', 'keep-alive');

    // 发送数据到客户端
    const sendEvent = (data) => {
        res.write(`data: ${data}\n\n`);
    };

    let key = getUniqueID();
    clients.set(key, sendEvent);

    req.on('close', () => {
        clients.delete(key);
        res.end();
    });
});

// 启动服务器
const PORT = 13000;
app.listen(PORT, () => {
    console.log(`Server is running on port ${PORT}`);
});

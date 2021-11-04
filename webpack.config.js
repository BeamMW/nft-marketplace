const path = require('path')
const html = require('html-webpack-plugin')
const copy = require("copy-webpack-plugin")
const extractCSS = require("mini-css-extract-plugin")

module.exports = {
    entry: {
        main: "./src/main.js",
    },
    devServer: {
        hot: true
    },
    output: {
        filename: "[name].bundle.js",
        path: path.resolve(__dirname, "html"),
        clean: true
    },
    module: {
        rules: [
            {
                test: /\.css$/i,
                use: [extractCSS.loader, "css-loader"],
            }
        ]
    },
    plugins: [
        new extractCSS({
        }),
        new html({
            title: "BEAM NFT Gallery",
            favicon: "./src/appicon.svg",
            scriptLoading: "blocking",
            meta: {
                viewport: "width=device-width,initial-scale=1.0",
            },
        }),
        new copy({
            patterns: [
                "./src/galleryManager.wasm",
                {from: "assets/**/*", context: "./src"}
            ]
        })
    ]
}

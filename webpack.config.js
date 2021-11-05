const path = require('path')
const html = require('html-webpack-plugin')
const copy = require("copy-webpack-plugin")
const extractCSS = require("mini-css-extract-plugin")

const config = (DEV_MODE) => {return {
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
                use: [
                    extractCSS.loader, 
                    {
                        loader: "css-loader",
                        options: {
                            sourceMap: DEV_MODE
                        }
                    }
                ],
            }
        ]
    },
    plugins: [
        new extractCSS({ 
        }),
        new html({
            templateContent: ({htmlWebpackPlugin}) => `
                <!DOCTYPE html>
                <html>
                <head>
                <meta charset="utf-8"/>
                <title>${htmlWebpackPlugin.options.title}</title>
                </head>
                <body>
                <script src="wasm-client.js"></script>
                </body>
                </html>
            `,
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
                "./src/appicon.svg",
                {   
                    from: "assets/**/*", 
                    context: "./src"
                },
                {
                    from: path.join(__dirname, './node_modules/beam-wasm-client-masternet/'),
                    globOptions: {
                        ignore: ['**/package.json', '**/README.md'],
                    },
                }
            ]
        })
    ],
    devtool: DEV_MODE ? "eval-source-map" : undefined,
    resolve: {
        alias: {
            // This is necessary for runtime template compilation
            // when we user vue template in a pure js, not in a *.vue component
            'vue': 'vue/dist/vue.esm-browser.js', 
        }
    }
}}

module.exports = (env, argv) => {
    const DEV_MODE = argv.mode == 'development'
    console.log(argv.mode)
    return config(DEV_MODE)
}

/*
 * ATTENTION: The "eval" devtool has been used (maybe by default in mode: "development").
 * This devtool is neither made for production nor for readable output files.
 * It uses "eval()" calls to create a separate source file in the browser devtools.
 * If you are trying to read the output file, select a different devtool (https://webpack.js.org/configuration/devtool/)
 * or disable the default devtool with "devtool: false".
 * If you are looking for production-ready output files, see mode: "production" (https://webpack.js.org/configuration/mode/).
 */
/******/ (() => { // webpackBootstrap
/******/ 	"use strict";
/******/ 	var __webpack_modules__ = ({

/***/ "./src/styles/fonts.css":
/*!******************************!*\
  !*** ./src/styles/fonts.css ***!
  \******************************/
/***/ ((__unused_webpack_module, __webpack_exports__, __webpack_require__) => {

eval("__webpack_require__.r(__webpack_exports__);\n// extracted by mini-css-extract-plugin\n\n\n//# sourceURL=webpack://beam-nft-marketplace/./src/styles/fonts.css?");

/***/ }),

/***/ "./src/styles/global.css":
/*!*******************************!*\
  !*** ./src/styles/global.css ***!
  \*******************************/
/***/ ((__unused_webpack_module, __webpack_exports__, __webpack_require__) => {

eval("__webpack_require__.r(__webpack_exports__);\n// extracted by mini-css-extract-plugin\n\n\n//# sourceURL=webpack://beam-nft-marketplace/./src/styles/global.css?");

/***/ }),

/***/ "./src/main.js":
/*!*********************!*\
  !*** ./src/main.js ***!
  \*********************/
/***/ ((__unused_webpack_module, __webpack_exports__, __webpack_require__) => {

eval("__webpack_require__.r(__webpack_exports__);\n/* harmony import */ var _utils_utils_js__WEBPACK_IMPORTED_MODULE_0__ = __webpack_require__(/*! ./utils/utils.js */ \"./src/utils/utils.js\");\n/* harmony import */ var _styles_fonts_css__WEBPACK_IMPORTED_MODULE_1__ = __webpack_require__(/*! ./styles/fonts.css */ \"./src/styles/fonts.css\");\n/* harmony import */ var _styles_global_css__WEBPACK_IMPORTED_MODULE_2__ = __webpack_require__(/*! ./styles/global.css */ \"./src/styles/global.css\");\n//import Vue   from \"vue\"\n//import App   from \"./App.vue\"\n\n\n\n\n_utils_utils_js__WEBPACK_IMPORTED_MODULE_0__[\"default\"].initialize({\n        appname: \"BEAM NFT Gallery\",\n        min_api_version: \"6.2\",\n        \"apiResultHandler\": () => {}\n    }, \n    (err) => {\n        //const vueApp = Vue.createApp(App)\n        //vueApp.mount('body')\n        alert(\"loaded2\")\n    }\n)\n\n/*import App      from './components/app.js'\nimport {store}  from './store.js'\nimport {router} from './router.js'\nimport utils    from './utils/utils.js'\n\nutils.initialize(\n    {\n        \"appname\": \"BEAM Gallery\",\n        \"min_api_version\": \"6.1\",\n        \"apiResultHandler\": (...args) => store.onApiResult(...args)\n    }, \n    (err) => {\n        const vueApp = Vue.createApp(App);\n        vueApp.config.globalProperties.$store = store;\n        vueApp.config.globalProperties.$state = Vue.readonly(store.state);\n        vueApp.use(router);\n        vueApp.mount('body');\n\n        const { validator_error, \n                content_main,\n                appsGradientTop,\n                appsGradientOffset,\n                background_main_top,\n                background_main\n        } = utils.getStyles();\n\n        if (utils.isWeb()) {\n            document.body.style.padding = '5%';\n        }\n        \n        const topColor =  [appsGradientOffset, \"px,\"].join('');\n        const mainColor = [appsGradientTop, \"px,\"].join('');\n        const style = document.createElement('style');\n        style.innerHTML = `.error {color: ${validator_error};}`;\n        document.head.appendChild(style);\n        document.body.style.color = content_main;\n\n        if (!utils.isDesktop()) {\n            document.body.style.backgroundImage = [\n                \"linear-gradient(to bottom,\",\n                background_main_top, topColor,\n                background_main, mainColor,\n                background_main\n            ].join(' ');\n        }\n        \n\n        if (err) {\n            return store.setError(err);\n        }\n\n        store.start();\n    }\n)\n*/\n\n//# sourceURL=webpack://beam-nft-marketplace/./src/main.js?");

/***/ }),

/***/ "./src/utils/utils.js":
/*!****************************!*\
  !*** ./src/utils/utils.js ***!
  \****************************/
/***/ ((__unused_webpack_module, __webpack_exports__, __webpack_require__) => {

eval("__webpack_require__.r(__webpack_exports__);\n/* harmony export */ __webpack_require__.d(__webpack_exports__, {\n/* harmony export */   \"default\": () => (/* binding */ Utils)\n/* harmony export */ });\nconst MIN_AMOUNT = 0.00000001;\nconst MAX_AMOUNT = 254000000;\n\nlet BEAM     = null\nlet CallID   = 0\nlet Calls    = {}\nlet APIResCB = undefined\n\nclass Utils {\n    static isMobile () {\n        const ua = navigator.userAgent;\n        return (/android/i.test(ua) || /iPad|iPhone|iPod/.test(ua));\n    }\n    \n    static isAndroid () {\n        const ua = navigator.userAgent;\n        return (/android/i.test(ua));\n    }\n\n    static isDesktop () {\n        const ua = navigator.userAgent;\n        return (/QtWebEngine/i.test(ua));\n    }\n\n    static isWeb () {\n        return !Utils.isDesktop() && !Utils.isMobile()\n    }\n\n    static async createDesktopAPI(apirescback) {\n        return new Promise(async (resolve, reject) => {\n            await Utils.injectScript(\"qrc:///qtwebchannel/qwebchannel.js\")\n            new QWebChannel(qt.webChannelTransport, (channel) => {\n                channel.objects.BEAM.api.callWalletApiResult.connect(apirescback)\n                resolve(channel.objects.BEAM)\n            })\n        })  \n    }\n\n    static async createWebAPI(apiver, apivermin, appname, apirescback) {\n        return new Promise((resolve, reject) => {\n            window.addEventListener('message', async (ev) => {\n                if (ev.data === 'apiInjected') {\n                    await window.BeamApi.callWalletApiResult(apirescback);\n                    resolve(window.BeamApi)\n                }\n            }, false);\n            window.postMessage({type: \"create_beam_api\", apiver, apivermin, appname}, window.origin);\n        })\n    }\n\n    static async createMobileAPI(apirescback) {\n        return new Promise((resolve, reject) => {\n            if (Utils.isAndroid()) {\n                document.addEventListener(\"onCallWalletApiResult\", (res) => {\n                    apirescback(res.detail)\n                })\n            }\n            else {\n                window.BEAM.callWalletApiResult(apirescback);\n            }\n            resolve(window.BEAM);\n        })\n    }\n\n    static async callApi(method, params, cback) {\n        let callid = ['call', CallID++].join('-')\n        Calls[callid] = cback\n\n        let request = {\n            \"jsonrpc\": \"2.0\",\n            \"id\":      callid,\n            \"method\":  method,\n            \"params\":  params\n        }\n\n        //console.log(Utils.formatJSON(request))\n\n        if (Utils.isWeb()) {\n            BEAM.callWalletApi(callid, method, params);\n        } \n\n        if (Utils.isMobile()) {\n            BEAM.callWalletApi(JSON.stringify(request));\n        }\n        \n        if (Utils.isDesktop()) {\n            BEAM.api.callWalletApi(JSON.stringify(request));\n        }\n    }\n\n    static invokeContract(args, cback, bytes) {\n        let params = {\n            \"create_tx\": false\n        }\n\n        if (args) {\n            params = Object.assign({\n                \"args\": args\n            }, params)\n        }\n\n        if (bytes) {\n            params = Object.assign({\n                \"contract\": bytes\n            }, params)\n        }\n\n        return Utils.callApi('invoke_contract', params, cback)\n    }\n\n    static handleApiResult (json) {\n        let answer = undefined\n        try\n        {\n            answer = JSON.parse(json);\n           \n            if (answer.result && answer.result.output) {\n                //console.log('Output: ', JSON.parse(answer.result.output));\n            } else {\n                //console.log('Api result: ', answer);\n            }\n            \n            const id = answer.id\n            const cback = Calls[id] || APIResCB\n            delete Calls[id]\n            \n            if (answer.error) {\n                return cback(answer)\n            }\n\n            if (!answer.result) {\n                return cback({\n                    error: \"no valid api call result\", \n                    answer\n                })\n            }\n\n            if (typeof answer.result.output == 'string') {\n                // this is shader result\n                let shaderAnswer = JSON.parse(answer.result.output)\n                if (shaderAnswer.error) {\n                    return cback({\n                        error: shaderAnswer.error,\n                        answer\n                    })\n                }\n                return cback(null, shaderAnswer, answer)\n            }\n            else\n            {\n                return cback(null, answer.result, answer)\n            }\n        }\n        catch (err)\n        {\n            APIResCB({\n                error: err.toString(),\n                answer: answer || json\n            })\n        }\n    }\n\n    static async initialize(params, initcback) {\n        APIResCB = params[\"apiResultHandler\"]\n        \n        try\n        {\n            if (Utils.isDesktop()) {\n                BEAM = await Utils.createDesktopAPI((...args) => Utils.handleApiResult(...args))\n            } \n            \n            if (Utils.isWeb()) {\n                Utils.showWebLoading()\n                let apiver    = params[\"api_version\"] || \"current\"\n                let apivermin = params[\"min_api_version\"] || \"\"\n                let appname   = params[\"appname\"]\n                BEAM = await Utils.createWebAPI(apiver, apivermin, appname, (...args) => Utils.handleApiResult(...args))\n                Utils.hideWebLoading()\n            }\n\n            if (Utils.isMobile()) {\n                BEAM = await Utils.createMobileAPI((...args) => Utils.handleApiResult(...args))\n            }\n\n            let styles = Utils.getStyles()\n            Utils.applyStyles(styles); \n        }\n        catch (err)\n        {\n            return initcback(err)\n        }\n\n        return initcback(null)\n    }\n\n    static getStyles () {\n        if (BEAM && BEAM.style) {\n            // TODO: проборосить стили из мобайла и экстеншена\n            return BEAM.style\n        }\n\n        return {\n            appsGradientOffset: -174,\n            appsGradientTop: 56,\n            content_main: \"#ffffff\",\n            background_main_top: \"#035b8f\",\n            background_main: \"#042548\",\n            background_popup: \"#00446c\",\n            validator_error: \"#ff625c\"\n        }\n    }\n\n    static applyStyles(style) {\n        if (!Utils.isDesktop()) {\n            document.head.innerHTML += '<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />';\n        }\n\n        if (Utils.isMobile()) {\n            document.body.classList.add('mobile');\n        }\n\n        if (Utils.isWeb()) {\n            document.body.classList.add('web');\n        }\n    }\n    \n    //\n    // Convenience functions\n    //\n    static reload () {\n        window.location.reload();\n    }\n    \n    static async injectScript(url) {\n        return new Promise((resolve, reject) => {\n            let js = document.createElement('script');\n            js.type = 'text/javascript';\n            js.async = true;\n            js.src = url;\n            js.onload = () => resolve()\n            js.onerror = (err) => reject(err)\n            document.getElementsByTagName('head')[0].appendChild(js);\n        })\n    }\n\n    static hex2rgba = (hex, alpha = 1) => {\n        const [r, g, b] = hex.match(/\\w\\w/g).map(x => parseInt(x, 16));\n        return `rgba(${r},${g},${b},${alpha})`;\n    };\n\n    static getById = (id)  => {\n        return document.getElementById(id);\n    }\n    \n    static setText(id, text) {\n        Utils.getById(id).innerText = text;\n    }\n\n    static show(id) {\n        Utils.getById(id).classList.remove(\"hidden\");\n    }\n    \n    static hide(id) {\n        Utils.getById(id).classList.add(\"hidden\");\n    }\n\n    static download(url, cback) {\n        var xhr = new XMLHttpRequest();\n        xhr.onreadystatechange = function() {\n            if(xhr.readyState === XMLHttpRequest.DONE) {\n                if (xhr.status === 200) {\n                    let buffer    = xhr.response;\n                    let byteArray = new Uint8Array(buffer);\n                    let array     = Array.from(byteArray);\n\n                    if (!array || !array.length) {\n                        return cback(\"empty shader\");\n                    }\n                \n                    return cback(null, array);\n                } else {\n                    let errMsg = [\"code\", xhr.status].join(\" \");\n                    return cback(errMsg);\n                }\n            }\n        }\n        xhr.open('GET', url, true);\n        xhr.responseType = \"arraybuffer\";\n        xhr.send(null);\n    }\n\n    static handleString(next) {\n        let result = true;\n        const regex = new RegExp(/^-?\\d+(\\.\\d*)?$/g);\n        const floatValue = parseFloat(next);\n        const afterDot = next.indexOf('.') > 0 ? next.substring(next.indexOf('.') + 1) : '0';\n        if ((next && !String(next).match(regex)) ||\n            (String(next).length > 1 && String(next)[0] === '0' && next.indexOf('.') < 0) ||\n            (parseInt(afterDot, 10) === 0 && afterDot.length > 7) ||\n            (afterDot.length > 8) ||\n            (floatValue === 0 && next.length > 1 && next[1] !== '.') ||\n            (floatValue < 1 && next.length > 10) ||\n            (floatValue > 0 && (floatValue < MIN_AMOUNT || floatValue > MAX_AMOUNT))) {\n          result = false;\n        }\n        return result;\n    }\n\n    static showWebLoading() {\n        let styles = Utils.getStyles()\n        Utils.applyStyles(styles);\n        const topColor =  [styles.appsGradientOffset, \"px,\"].join('');\n        const mainColor = [styles.appsGradientTop, \"px,\"].join('');\n\n        let bg = document.createElement(\"div\");\n        bg.style.width = \"100%\";\n        bg.style.height = \"100%\";\n        bg.style.color = \"#fff\";\n        bg.id = \"dapp-loader\";\n        bg.style.position = \"absolute\";\n        bg.style.backgroundImage = [\n            \"linear-gradient(to bottom,\",\n            styles.background_main_top, topColor,\n            styles.background_main, mainColor,\n            styles.background_main\n        ].join(' ');\n        let loadContainer = document.createElement(\"div\");\n        loadContainer.className = \"dapp-loading\";\n\n        loadContainer.style.textAlign = 'center';\n        loadContainer.style.margin = '50px auto 0 auto';\n        loadContainer.style.width = '585px';\n        loadContainer.style.padding = '5%';\n        loadContainer.style.backgroundColor = 'rgba(255, 255, 255, 0.05)';\n        loadContainer.style.borderRadius = '10px';\n\n        let titleElem = document.createElement(\"h3\");\n        titleElem.innerText = \"Connecting to BEAM Web Wallet.\"; \n        let subtitle = document.createElement(\"p\");\n        subtitle.innerText = \"To use BEAM Gallery you should have BEAM Web Wallet installed and allow connection.\";\n\n        let reconnectButton = document.createElement(\"button\");\n        reconnectButton.innerText = \"Try to connect again\";\n        reconnectButton.style.height = \"44px\";\n        reconnectButton.style.padding = \"13px 30px\";\n        reconnectButton.style.borderRadius = \"50px\";\n        reconnectButton.style.border = \"none\";\n        reconnectButton.style.color = \"#fff\";\n        reconnectButton.style.cursor = \"pointer\";\n        reconnectButton.style.fontWeight = \"bold\";\n        reconnectButton.style.fontSize = \"14px\";\n        reconnectButton.style.backgroundColor = \"rgba(255, 255, 255, 0.1)\";\n\n        reconnectButton.addEventListener(\"mouseover\", () => {\n            reconnectButton.style.boxShadow = \"0 0 8px white\";\n        }, false);\n        reconnectButton.addEventListener(\"mouseout\", () => {\n            reconnectButton.style.boxShadow = \"none\";\n        }, false);\n\n\n        reconnectButton.addEventListener('click', () => {\n            Utils.reload();\n        });\n        let installButton = document.createElement(\"button\");\n        installButton.innerText = \"Install BEAM Web Wallet\";\n        installButton.style.height = \"44px\";\n        installButton.style.padding = \"13px 30px\";\n        installButton.style.borderRadius = \"50px\";\n        installButton.style.border = \"none\";\n        installButton.style.color = \"#042548\";\n        installButton.style.cursor = \"pointer\";\n        installButton.style.fontWeight = \"bold\";\n        installButton.style.fontSize = \"14px\";\n        installButton.style.backgroundColor = \"#00f6d2\";\n        installButton.addEventListener('click', () => {\n            window.open('https://chrome.google.com/webstore/detail/beam-web-wallet/ilhaljfiglknggcoegeknjghdgampffk', \n                '_blank');\n        });\n\n        installButton.addEventListener(\"mouseover\", () => {\n            installButton.style.boxShadow = \"0 0 8px white\";\n        }, false);\n        installButton.addEventListener(\"mouseout\", () => {\n            installButton.style.boxShadow = \"none\";\n        }, false);\n        installButton.style.marginLeft = '30px';\n        \n        let controlsArea = document.createElement(\"div\");\n        controlsArea.style.marginTop = \"50px\";\n        \n        loadContainer.appendChild(titleElem);\n        loadContainer.appendChild(subtitle);\n        loadContainer.appendChild(controlsArea);\n\n        controlsArea.appendChild(reconnectButton);\n        controlsArea.appendChild(installButton);\n\n        bg.appendChild(loadContainer);\n\n        document.body.appendChild(bg)\n    }\n\n    static hideWebLoading() {\n        const elem = document.getElementById(\"dapp-loader\");\n        elem.parentNode.removeChild(elem);\n    }\n\n    static formateValue(value) {\n        if (value > 0) {\n            return parseFloat(value.toFixed(2)).toString();\n        } else {\n            return value;\n        }\n    }\n\n    static numberWithCommas(x) {\n        if (x > 0) {\n            return x.replace(/\\B(?=(\\d{3})+(?!\\d))/g, \",\");\n        } else {\n            return x;\n        }\n    }\n\n    static getRateStr(value, rate) {\n        const rateVal = Utils.formateValue(new Big(value).times(rate));\n        return (rate > 0 && value > 0\n          ? (rateVal > 0.1 ? (Utils.numberWithCommas(rateVal) + ' USD') : '< 1 cent')\n          : '0 USD');\n    }\n\n    static ensureField(obj, name, type) {\n        if (obj[name] == undefined) {\n            throw `No '${name}' field on object`\n        }\n\n        if (type == 'array') {\n            if (!Array.isArray(obj[name])) {\n                throw `${name} is expected to be an array`\n            }\n            return\n        }\n\n        if (type) {\n            let tof = typeof obj[name]\n            if (tof !== type) {\n                throw `Bad type '${tof}' for '${name}'. '${type}' expected.`\n            }\n            return\n        }\n    }\n\n    static isUserCancelled (err) {\n        return err.error && err.error.code == -32021\n    }\n\n    static formatJSON(obj) {\n        let res = JSON.stringify(obj, null, 2)\n        return res == \"{}\" ? obj.toString() : res\n    }\n}\n\n\n//# sourceURL=webpack://beam-nft-marketplace/./src/utils/utils.js?");

/***/ })

/******/ 	});
/************************************************************************/
/******/ 	// The module cache
/******/ 	var __webpack_module_cache__ = {};
/******/ 	
/******/ 	// The require function
/******/ 	function __webpack_require__(moduleId) {
/******/ 		// Check if module is in cache
/******/ 		var cachedModule = __webpack_module_cache__[moduleId];
/******/ 		if (cachedModule !== undefined) {
/******/ 			return cachedModule.exports;
/******/ 		}
/******/ 		// Create a new module (and put it into the cache)
/******/ 		var module = __webpack_module_cache__[moduleId] = {
/******/ 			// no module.id needed
/******/ 			// no module.loaded needed
/******/ 			exports: {}
/******/ 		};
/******/ 	
/******/ 		// Execute the module function
/******/ 		__webpack_modules__[moduleId](module, module.exports, __webpack_require__);
/******/ 	
/******/ 		// Return the exports of the module
/******/ 		return module.exports;
/******/ 	}
/******/ 	
/************************************************************************/
/******/ 	/* webpack/runtime/define property getters */
/******/ 	(() => {
/******/ 		// define getter functions for harmony exports
/******/ 		__webpack_require__.d = (exports, definition) => {
/******/ 			for(var key in definition) {
/******/ 				if(__webpack_require__.o(definition, key) && !__webpack_require__.o(exports, key)) {
/******/ 					Object.defineProperty(exports, key, { enumerable: true, get: definition[key] });
/******/ 				}
/******/ 			}
/******/ 		};
/******/ 	})();
/******/ 	
/******/ 	/* webpack/runtime/hasOwnProperty shorthand */
/******/ 	(() => {
/******/ 		__webpack_require__.o = (obj, prop) => (Object.prototype.hasOwnProperty.call(obj, prop))
/******/ 	})();
/******/ 	
/******/ 	/* webpack/runtime/make namespace object */
/******/ 	(() => {
/******/ 		// define __esModule on exports
/******/ 		__webpack_require__.r = (exports) => {
/******/ 			if(typeof Symbol !== 'undefined' && Symbol.toStringTag) {
/******/ 				Object.defineProperty(exports, Symbol.toStringTag, { value: 'Module' });
/******/ 			}
/******/ 			Object.defineProperty(exports, '__esModule', { value: true });
/******/ 		};
/******/ 	})();
/******/ 	
/************************************************************************/
/******/ 	
/******/ 	// startup
/******/ 	// Load entry module and return exports
/******/ 	// This entry module can't be inlined because the eval devtool is used.
/******/ 	var __webpack_exports__ = __webpack_require__("./src/main.js");
/******/ 	
/******/ })()
;
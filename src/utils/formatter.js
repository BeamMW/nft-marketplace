export default function formatter(num) {
  const fixedNum = num.toFixed(3)

  if (num < 0.01) {
    return '< 0.01'
  }

  if (num >= 0.01 && fixedNum < 1000) {
    return +num.toFixed(2)
  }

  if (fixedNum >= 1000 && fixedNum < 1000000) {
    const head = fixedNum.slice(0, -7)
    const tail = fixedNum.slice(-7, -5)
    const n = +(head + '.' + tail)

    return n + ' k'
  }

  if (fixedNum >= 1000000 && fixedNum < 1000000000) {
    const head = fixedNum.slice(0, -10)
    const tail = fixedNum.slice(-10, -8)
    const n = +(head + '.' + tail)

    return n + ' m'
  }

  if (fixedNum >= 1000000000) {
    const head = fixedNum.slice(0, -13)
    const tail = fixedNum.slice(-13, -11)
    const n = +(head + '.' + tail)

    return n + ' b'
  }

  return 'error'
}
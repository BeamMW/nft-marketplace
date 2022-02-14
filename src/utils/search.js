
export function binary_search (arr, compare, skip = 0) {
  let binary_find = function (left, right) {
    if (left > right) return -1
    let mid = Math.floor((left + right) / 2)
    let cmp = compare(arr[mid])
    if (cmp == 0) return mid
    if (cmp < 0) return binary_find(left, mid - 1)
    return binary_find(mid + 1, right)
  }
  return binary_find(skip, arr.length - 1)
}

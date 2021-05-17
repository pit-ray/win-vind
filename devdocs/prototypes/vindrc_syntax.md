# Syntax
## File format
Support only UTF-8

## Set option
ignore space, deliminator is "\n"

```vim
set {name}
set no{name}
set {name} = {val}
set {name} : {val}
```

### Value Type

|Type|Value|
|:---:|:---:|
|bool(int)|zero, non-zero|
|int|32bit|
|string|"str", 'str'|

## Custom Keybindings

### Syntax
```vim
{map-cmd} {new-binding} {key}
{map-cmd} {new-binding} {combined-keys}
{map-cmd} {new-binding} {keys-sequence}
{map-cmd} {new-binding} {func-id}
```

**Example**
```vim
inoremap <capslock> <ctrl>
enn <space> <ctrl-f>
ino B g<c-l>yy
gnnoremap <esc-left> change_to_normal
```

### Map

|Syntax|Mode|
|:---:|:---:|
|`gnm[ap]`|GUI Normal|
|`gvm[ap]`|GUI Visual|
|`enm[ap]`|Edi Normal|
|`evm[ap]`|Edi Visual|
|`im[ap]`|Insert|
|`cm[ap]`|Command|


### Noremap

|Syntax|Mode|
|:---:|:---:|
|`gnn[oremap]`|GUI Normal|
|`gvn[oremap]`|GUI Visual|
|`enn[oremap]`|Edi Normal|
|`evn[oremap]`|Edi Visual|
|`ino[remap]`|Insert|
|`cno[remap]`|Command|


### Delete Map

|Syntax|Mode|
|:---:|:---:|
|`gnun[map]`|GUI Normal|
|`gvun[map]`|GUI Visual|
|`enun[map]`|Edi Normal|
|`evun[map]`|Edi Visual|
|`iu[nmap]`|Insert|
|`cu[nmap]`|Command|

### Clear Map

|Syntax|Mode|
|:---:|:---:|
|`gnmapc[lear]`|GUI Normal|
|`gvmapc[lear]`|GUI Visual|
|`enmapc[lear]`|Edi Normal|
|`evmapc[lear]`|Edi Visual|
|`imapc[lear]`|Insert|
|`cmapc[lear]`|Command|

### Command Add/Del/Clear
|Syntax|Mode|
|:---:|:---:|
|`com[mand]`|Command|
|`delc[ommand]`|Command|
|`comc[lear]`|Command|


## Differences

### Mathcing

```vim
imap aa foo
imap aaa bar
```
bar will not be called.

### User-defined Commands

You can use user-defined command began with lowercase except for `:set` or `:source`.

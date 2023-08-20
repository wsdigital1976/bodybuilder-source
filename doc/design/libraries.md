# Libraries

| Name                     | Description |
|--------------------------|-------------|
| *libbodybuilder_cli*         | RPC client functionality used by *bodybuilder-cli* executable |
| *libbodybuilder_common*      | Home for common functionality shared by different executables and libraries. Similar to *libbodybuilder_util*, but higher-level (see [Dependencies](#dependencies)). |
| *libbodybuilder_consensus*   | Stable, backwards-compatible consensus functionality used by *libbodybuilder_node* and *libbodybuilder_wallet* and also exposed as a [shared library](../shared-libraries.md). |
| *libbodybuilderconsensus*    | Shared library build of static *libbodybuilder_consensus* library |
| *libbodybuilder_kernel*      | Consensus engine and support library used for validation by *libbodybuilder_node* and also exposed as a [shared library](../shared-libraries.md). |
| *libbodybuilderqt*           | GUI functionality used by *bodybuilder-qt* and *bodybuilder-gui* executables |
| *libbodybuilder_ipc*         | IPC functionality used by *bodybuilder-node*, *bodybuilder-wallet*, *bodybuilder-gui* executables to communicate when [`--enable-multiprocess`](multiprocess.md) is used. |
| *libbodybuilder_node*        | P2P and RPC server functionality used by *bodybuilderd* and *bodybuilder-qt* executables. |
| *libbodybuilder_util*        | Home for common functionality shared by different executables and libraries. Similar to *libbodybuilder_common*, but lower-level (see [Dependencies](#dependencies)). |
| *libbodybuilder_wallet*      | Wallet functionality used by *bodybuilderd* and *bodybuilder-wallet* executables. |
| *libbodybuilder_wallet_tool* | Lower-level wallet functionality used by *bodybuilder-wallet* executable. |
| *libbodybuilder_zmq*         | [ZeroMQ](../zmq.md) functionality used by *bodybuilderd* and *bodybuilder-qt* executables. |

## Conventions

- Most libraries are internal libraries and have APIs which are completely unstable! There are few or no restrictions on backwards compatibility or rules about external dependencies. Exceptions are *libbodybuilder_consensus* and *libbodybuilder_kernel* which have external interfaces documented at [../shared-libraries.md](../shared-libraries.md).

- Generally each library should have a corresponding source directory and namespace. Source code organization is a work in progress, so it is true that some namespaces are applied inconsistently, and if you look at [`libbodybuilder_*_SOURCES`](../../src/Makefile.am) lists you can see that many libraries pull in files from outside their source directory. But when working with libraries, it is good to follow a consistent pattern like:

  - *libbodybuilder_node* code lives in `src/node/` in the `node::` namespace
  - *libbodybuilder_wallet* code lives in `src/wallet/` in the `wallet::` namespace
  - *libbodybuilder_ipc* code lives in `src/ipc/` in the `ipc::` namespace
  - *libbodybuilder_util* code lives in `src/util/` in the `util::` namespace
  - *libbodybuilder_consensus* code lives in `src/consensus/` in the `Consensus::` namespace

## Dependencies

- Libraries should minimize what other libraries they depend on, and only reference symbols following the arrows shown in the dependency graph below:

<table><tr><td>

```mermaid

%%{ init : { "flowchart" : { "curve" : "basis" }}}%%

graph TD;

bodybuilder-cli[bodybuilder-cli]-->libbodybuilder_cli;

bodybuilderd[bodybuilderd]-->libbodybuilder_node;
bodybuilderd[bodybuilderd]-->libbodybuilder_wallet;

bodybuilder-qt[bodybuilder-qt]-->libbodybuilder_node;
bodybuilder-qt[bodybuilder-qt]-->libbodybuilderqt;
bodybuilder-qt[bodybuilder-qt]-->libbodybuilder_wallet;

bodybuilder-wallet[bodybuilder-wallet]-->libbodybuilder_wallet;
bodybuilder-wallet[bodybuilder-wallet]-->libbodybuilder_wallet_tool;

libbodybuilder_cli-->libbodybuilder_util;
libbodybuilder_cli-->libbodybuilder_common;

libbodybuilder_common-->libbodybuilder_consensus;
libbodybuilder_common-->libbodybuilder_util;

libbodybuilder_kernel-->libbodybuilder_consensus;
libbodybuilder_kernel-->libbodybuilder_util;

libbodybuilder_node-->libbodybuilder_consensus;
libbodybuilder_node-->libbodybuilder_kernel;
libbodybuilder_node-->libbodybuilder_common;
libbodybuilder_node-->libbodybuilder_util;

libbodybuilderqt-->libbodybuilder_common;
libbodybuilderqt-->libbodybuilder_util;

libbodybuilder_wallet-->libbodybuilder_common;
libbodybuilder_wallet-->libbodybuilder_util;

libbodybuilder_wallet_tool-->libbodybuilder_wallet;
libbodybuilder_wallet_tool-->libbodybuilder_util;

classDef bold stroke-width:2px, font-weight:bold, font-size: smaller;
class bodybuilder-qt,bodybuilderd,bodybuilder-cli,bodybuilder-wallet bold
```
</td></tr><tr><td>

**Dependency graph**. Arrows show linker symbol dependencies. *Consensus* lib depends on nothing. *Util* lib is depended on by everything. *Kernel* lib depends only on consensus and util.

</td></tr></table>

- The graph shows what _linker symbols_ (functions and variables) from each library other libraries can call and reference directly, but it is not a call graph. For example, there is no arrow connecting *libbodybuilder_wallet* and *libbodybuilder_node* libraries, because these libraries are intended to be modular and not depend on each other's internal implementation details. But wallet code is still able to call node code indirectly through the `interfaces::Chain` abstract class in [`interfaces/chain.h`](../../src/interfaces/chain.h) and node code calls wallet code through the `interfaces::ChainClient` and `interfaces::Chain::Notifications` abstract classes in the same file. In general, defining abstract classes in [`src/interfaces/`](../../src/interfaces/) can be a convenient way of avoiding unwanted direct dependencies or circular dependencies between libraries.

- *libbodybuilder_consensus* should be a standalone dependency that any library can depend on, and it should not depend on any other libraries itself.

- *libbodybuilder_util* should also be a standalone dependency that any library can depend on, and it should not depend on other internal libraries.

- *libbodybuilder_common* should serve a similar function as *libbodybuilder_util* and be a place for miscellaneous code used by various daemon, GUI, and CLI applications and libraries to live. It should not depend on anything other than *libbodybuilder_util* and *libbodybuilder_consensus*. The boundary between _util_ and _common_ is a little fuzzy but historically _util_ has been used for more generic, lower-level things like parsing hex, and _common_ has been used for bodybuilder-specific, higher-level things like parsing base58. The difference between util and common is mostly important because *libbodybuilder_kernel* is not supposed to depend on *libbodybuilder_common*, only *libbodybuilder_util*. In general, if it is ever unclear whether it is better to add code to *util* or *common*, it is probably better to add it to *common* unless it is very generically useful or useful particularly to include in the kernel.


- *libbodybuilder_kernel* should only depend on *libbodybuilder_util* and *libbodybuilder_consensus*.

- The only thing that should depend on *libbodybuilder_kernel* internally should be *libbodybuilder_node*. GUI and wallet libraries *libbodybuilderqt* and *libbodybuilder_wallet* in particular should not depend on *libbodybuilder_kernel* and the unneeded functionality it would pull in, like block validation. To the extent that GUI and wallet code need scripting and signing functionality, they should be get able it from *libbodybuilder_consensus*, *libbodybuilder_common*, and *libbodybuilder_util*, instead of *libbodybuilder_kernel*.

- GUI, node, and wallet code internal implementations should all be independent of each other, and the *libbodybuilderqt*, *libbodybuilder_node*, *libbodybuilder_wallet* libraries should never reference each other's symbols. They should only call each other through [`src/interfaces/`](`../../src/interfaces/`) abstract interfaces.

## Work in progress

- Validation code is moving from *libbodybuilder_node* to *libbodybuilder_kernel* as part of [The libbodybuilderkernel Project #24303](https://github.com/bodybuilder/bodybuilder/issues/24303)
- Source code organization is discussed in general in [Library source code organization #15732](https://github.com/bodybuilder/bodybuilder/issues/15732)

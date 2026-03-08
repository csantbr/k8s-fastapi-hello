# C++ TCP Server on Kubernetes

Microserviço TCP de alta performance em C++ com deploy automatizado via ArgoCD em Kubernetes local.

## Stack

- **Linguagem**: C++17
- **Container**: Docker (multi-stage build com GCC)
- **Orquestração**: Kubernetes (Kind)
- **GitOps**: ArgoCD
- **Healthcheck**: TCP Socket Probe

## Estrutura

```
├── app/
│   ├── main.cpp              # Servidor TCP
│   ├── Dockerfile            # Build multi-stage
│   └── .dockerignore
├── k8s-manifests/
│   ├── deployment.yaml       # Deployment (2 réplicas)
│   ├── service.yaml          # Service NodePort
│   └── kustomization.yaml
├── kind-config.yaml          # Cluster config
└── argocd-app.yaml           # GitOps Application
```

## Endpoints

| Porta | Protocolo | Descrição |
|-------|-----------|-----------|
| 30080 | TCP | Serviço exposto |
| 8000 | TCP | Container port / Healthcheck |

**Resposta**: `{"status":"ok","service":"cpp-server"}`

## ArgoCD

- **URL**: https://localhost:8080
- **Usuário**: `admin`
- **Senha**: `0XajmEgsIo-dZZ3W`

## Comandos

```powershell
# Status dos pods
kubectl get pods -l app=cpp-server

# Logs
kubectl logs -l app=cpp-server -f

# Restart
kubectl rollout restart deployment/cpp-server

# Build e deploy
docker build -t cpp-server:v1 ./app
kind load docker-image cpp-server:v1 --name cpp-cluster
kubectl rollout restart deployment/cpp-server

# ArgoCD UI
kubectl port-forward svc/argocd-server -n argocd 8080:443

# Deletar cluster
kind delete cluster --name cpp-cluster
```

## Licença

MIT
